/*

Copyright (c) 2026 MEGA Unit

This software is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

#define STB_IMAGE_IMPLEMENTATION
#include "incl/stb/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#define STB_TRUETYPE_IMPLEMENTATION
#include "incl/stb/stb_truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION

#define MINIAUDIO_IMPLEMENTATION
#include "incl/miniaudio.h"
#undef MINIAUDIO_IMPLEMENTATION

#include "plates.h"

struct GetName {
    const std::string& operator()(const Box&         e) const { return e.name; }
    const std::string& operator()(const Circle&      e) const { return e.name; }
    const std::string& operator()(const Text&        e) const { return e.name; }
    const std::string& operator()(const Image&       e) const { return e.name; }
    const std::string& operator()(const InputBox&    e) const { return e.name; }
    const std::string& operator()(const CheckBox&    e) const { return e.name; }
    const std::string& operator()(const RadioButton& e) const { return e.name; }
    const std::string& operator()(const Line&        e) const { return e.name; }
    const std::string& operator()(const Slider&      e) const { return e.name; }
    const std::string& operator()(const RangeSlider& e) const { return e.name; }
    const std::string& operator()(const ScrollBar&   e) const { return e.name; }
};

static std::filesystem::path getExecutableDir() {
    #ifdef _WIN32
        char buf[MAX_PATH];
        GetModuleFileNameA(nullptr, buf, MAX_PATH);
        return std::filesystem::path(buf).parent_path();
    #elif defined(__linux__)
        char buf[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
        if (len != -1) {
            buf[len] = '\0';
            return std::filesystem::path(buf).parent_path();
        }
        return std::filesystem::current_path();
    #else
        return std::filesystem::current_path();
    #endif
}

bool isInScrollBar(const std::string& name, ScrollBar** outSB = nullptr) {
    for (auto& el : mainSpace.uiElements) {
        if (!std::holds_alternative<ScrollBar>(el)) continue;
        ScrollBar& sb = std::get<ScrollBar>(el);
        for (auto& en : sb.elements)
            if (en == name) { if (outSB) *outSB = &sb; return true; }
    }
    return false;
}

bool parseNums4(std::string s, int16_t& a, int16_t& b, int16_t& c, int16_t& d) {
    for (char& ch : s) if (ch=='{'||ch=='}'||ch==',') ch=' ';
    std::istringstream ss(s);

    return !!(ss >> a >> b >> c >> d);
}

bool parseNums3(std::string s, int16_t& a, int16_t& b, int16_t& c) {
    for (char& ch : s) if (ch=='{'||ch=='}'||ch==',') ch=' ';
    std::istringstream ss(s);

    return !!(ss >> a >> b >> c);
}

std::string resolveFontPath(const std::string& path) {
    auto tryResolve = [&](const std::string& base, std::string& outPath) {
        if (base.find('.') == std::string::npos) {
            if (std::filesystem::exists(base + ".ttf")) { outPath = base + ".ttf"; return true; }
            if (std::filesystem::exists(base + ".otf")) { outPath = base + ".otf"; return true; }
            return false;
        }
        if (std::filesystem::exists(base)) { outPath = base; return true; }
        return false;
    };
    
    if (path.find('/') != std::string::npos ||
        path.find('\\') != std::string::npos ||
        (path.size() >= 2 && path[1] == ':'))
        return path;

    std::string result;

    std::filesystem::path exeDir = getExecutableDir();
    std::string localBase = (exeDir / "Fonts" / path).string();
    if (tryResolve(localBase, result)) return result;

    std::string fontFolder = "Fonts";
    std::string cwdBase = fontFolder + "/" + path;

    if (tryResolve(cwdBase, result)) return result;
    #ifdef _WIN32
        const char* userProfile = std::getenv("USERPROFILE");
        std::vector<std::string> systemDirs = {
            "C:\\Program Files (x86)\\Plates\\" + fontFolder + "\\",
            "C:\\Program Files\\Plates\\" + fontFolder + "\\",
        };
        if (userProfile) {
            systemDirs.push_back(std::string(userProfile) + "\\AppData\\Local\\Programs\\Plates\\" + fontFolder + "\\");
        }
    #elif defined(__linux__)
        const char* home = std::getenv("HOME");
        std::vector<std::string> systemDirs = {
            "/usr/include/Plates/" + fontFolder + "/",
            "/usr/lib/Plates/" + fontFolder + "/",
            "/usr/lib64/Plates/" + fontFolder + "/"
        };

        if (home) {
            systemDirs.push_back(std::string(home) + "/.local/include/Plates/" + fontFolder + "/");
            systemDirs.push_back(std::string(home) + "/.local/lib/Plates/" + fontFolder + "/");
        }
    #else
        std::vector<std::string> systemDirs;
    #endif
        for (const auto& dir : systemDirs) {
            std::string base = dir + path;

            if (tryResolve(base, result)) return result;
        }

        return cwdBase;
}

void drawBox(Box& box, bool ownDraw, int offX, int offY) {
    auto getRoundedBoxTexture = [&](SDL_Renderer* renderer, int w, int h, int rad, Color color) {
        auto makeTarget = [&](int tw, int th) {
            SDL_Texture* t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, tw, th);
            SDL_SetTextureScaleMode(t, SDL_ScaleModeLinear);
            return t;
        };
        
        const int scale = 4;

        SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);

        SDL_Texture* tex4x = makeTarget(w*scale, h*scale);
        SDL_SetTextureBlendMode(tex4x, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer, tex4x);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        roundedBoxRGBA(renderer, 0, 0, w*scale, h*scale, rad*scale,
            color.r, color.g, color.b, color.a);

        SDL_Texture* tex2x = makeTarget(w*2, h*2);
        SDL_SetTextureBlendMode(tex2x, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer, tex2x);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_SetTextureBlendMode(tex4x, SDL_BLENDMODE_NONE);
        SDL_Rect dst2x = {0,0, w*2, h*2};
        SDL_RenderCopy(renderer, tex4x, nullptr, &dst2x);
        SDL_DestroyTexture(tex4x);

        SDL_Texture* tex1x = makeTarget(w, h);
        SDL_SetTextureBlendMode(tex1x, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer, tex1x);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_SetTextureBlendMode(tex2x, SDL_BLENDMODE_NONE);
        SDL_Rect dst1x = {0,0, w, h};
        SDL_RenderCopy(renderer, tex2x, nullptr, &dst1x);
        SDL_DestroyTexture(tex2x);

        SDL_SetRenderTarget(renderer, prevTarget);
        return tex1x;
    };
    
    if (box.rect.w == 0 || box.rect.h == 0 || !box.Visible) return;

    if (!box.win->isVulkan) {
        if (box.aaDirty || !box.aaCache) {
            if (box.aaCache) SDL_DestroyTexture(box.aaCache);
            box.aaCache = getRoundedBoxTexture(box.win->renderer,
                box.rect.w, box.rect.h, box.rad, box.color);
            box.aaDirty = false;
        }
        
        SDL_Rect dst = {box.rect.x+offX, box.rect.y+offY, box.rect.w, box.rect.h};
        SDL_RenderCopy(box.win->renderer, box.aaCache, nullptr, &dst);
    } else {
        box.win->eng.drawRoundedRectV(box.rect.x+offX, box.rect.y+offY, box.rect.w, box.rect.h, box.rad, box.color);
    }

    if (ownDraw) {
        box.ownDraw = true;
    } else {
        box.ownDraw = false;
    }
}

void drawCircle(Circle& cir, bool ownDraw, int offX, int offY) {
    if (cir.uRect.size == 0 || !cir.Visible) return;

    if (!cir.win->isVulkan) {
        filledCircleRGBA(cir.win->renderer, cir.uRect.x+offX, cir.uRect.y+offY, cir.uRect.size,
            cir.clr.r, cir.clr.g, cir.clr.b, cir.clr.a);
        aacircleRGBA(cir.win->renderer, cir.uRect.x+offX, cir.uRect.y+offY, cir.uRect.size,
            cir.clr.r, cir.clr.g, cir.clr.b, cir.clr.a);
    } else {
        cir.win->eng.drawCircleV((float)(cir.uRect.x+offX), (float)(cir.uRect.y+offY), (float)cir.uRect.size, cir.clr);
    }
    
    if (ownDraw) {
        cir.ownDraw = true;
    } else {
        cir.ownDraw = false;
    }
}

void drawText(Text& txt, bool ownDraw, int offX, int offY) {
    if (txt.rect.w == 0 || txt.rect.h == 0 || !txt.Visible) return;

    if (!txt.win->isVulkan) {
        if (!txt.tex) return;
        SDL_Rect dst = {txt.rect.x+offX, txt.rect.y+offY, txt.surf->w, txt.surf->h};
        SDL_RenderCopy(txt.win->renderer, txt.tex, nullptr, &dst);
    } else {
        txt.win->eng.drawTextV(txt.pathToFont.c_str(), txt.text.c_str(), txt.fontSize,
            (float)(txt.rect.x+offX), (float)(txt.rect.y+offY), txt.color);
    }
}

void drawImage(Image& img, bool ownDraw, int offX, int offY) {
    if (img.rect.w == 0 || img.rect.h == 0 || !img.Visible) return;

    if (!img.win->isVulkan) {
        SDL_Rect r = {img.rect.x+offX, img.rect.y+offY, img.rect.w, img.rect.h};
        SDL_RenderCopy(img.win->renderer, img.tex, nullptr, &r);
    } else {
        img.win->eng.drawImageV(img.pathToImage.c_str(),
            (float)(img.rect.x+offX), (float)(img.rect.y+offY), img.rect.w, img.rect.h, img.frame);
    }
}

void drawLine(Line& ln, bool ownDraw, int offX, int offY) {
    if (ln.cords.w == 0 || ln.cords.h == 0 || !ln.Visible) return;

    if (!ln.win->isVulkan) {
        if (ln.win->renderer != nullptr) SDL_SetRenderDrawColor(ln.win->renderer, ln.color.r, ln.color.g, ln.color.b, ln.color.a);
        SDL_RenderDrawLine(ln.win->renderer, ln.cords.x+offX, ln.cords.y+offY, ln.cords.w+offX, ln.cords.h+offY);
    } else {
        ln.win->eng.drawLineV(ln.cords.x+offX, ln.cords.y+offY, ln.cords.w+offX, ln.cords.h+offY, ln.color);
    }

    if (ownDraw) {
        ln.ownDraw = true;
    } else {
        ln.ownDraw = false;
    }
}

void drawInputBox(InputBox& box, bool ownDraw, int offX, int offY) {
    if (box.rect.w == 0 || box.rect.h == 0 || !box.Visible) return;

    if (!box.win->isVulkan) {
        if (!box.font) { std::cerr << "ERROR: InputBox '" << box.name << "' has no font!\n"; return; }

        SDL_Rect clip = {box.rect.x+offX, box.rect.y+offY, box.rect.w, box.rect.h};
        SDL_RenderSetClipRect(box.win->renderer, &clip);

        if (box.debuggingBox) {
            roundedBoxRGBA(box.win->renderer,
                box.rect.x+offX, box.rect.y+offY,
                box.rect.x+offX+box.rect.w, box.rect.y+offY+box.rect.h,
                0, 70, 70, 70, 255);
        }

        auto renderText = [&](const std::string& s, SDL_Color c, int x, int y){
            SDL_Surface* sf = TTF_RenderUTF8_Blended_Wrapped(box.font, s.c_str(), c, 0);
            if (!sf) return;
            SDL_Texture* tx = SDL_CreateTextureFromSurface(box.win->renderer, sf);
            if (tx) {
                SDL_Rect d = {x, y, sf->w, sf->h};
                SDL_RenderCopy(box.win->renderer, tx, nullptr, &d);
                SDL_DestroyTexture(tx);
            }
            SDL_FreeSurface(sf);
        };

        if (!box.buf.empty())
            renderText(box.buf, {box.clr.r,box.clr.g,box.clr.b,box.clr.a},
                box.rect.x+offX, box.rect.y+offY);
        else if (!box.focus && !box.hint.empty()) {
            SDL_Surface* sf = TTF_RenderUTF8_Blended(box.font, box.hint.c_str(), {170,170,170,170});

            if (sf) {
                SDL_Texture* tx = SDL_CreateTextureFromSurface(box.win->renderer, sf);
                if (tx) {
                    SDL_Rect d = {box.rect.x+offX, box.rect.y+offY, sf->w, sf->h};
                    SDL_RenderCopy(box.win->renderer, tx, nullptr, &d);
                    SDL_DestroyTexture(tx);
                }
                SDL_FreeSurface(sf);
            }
        }

        int lineSkip = TTF_FontLineSkip(box.font);

        if (box.focus) {
            float t = SDL_GetTicks() / 1000.0f;
            float phase = sinf(t * 3.14159f * 2.0f / 1.0f);
            uint8_t caretAlpha = (uint8_t)(((phase + 1.0f) / 2.0f) * box.clr.a);

            size_t lineStart = box.buf.rfind('\n', box.caret > 0 ? box.caret - 1 : 0);
            lineStart = (lineStart == std::string::npos) ? 0 : lineStart + 1;

            int lineIndex = (int)std::count(box.buf.begin(),
                                             box.buf.begin() + std::min((size_t)box.caret, box.buf.size()),
                                             '\n');

            std::string curLine = box.buf.substr(lineStart, box.caret - lineStart);

            int lineW = 0, lineH = 0;
            if (!curLine.empty()) TTF_SizeUTF8(box.font, curLine.c_str(), &lineW, &lineH);

            int caretFinalX = box.rect.x + offX + lineW;
            int caretFinalY = box.rect.y + offY + lineIndex * lineSkip;

            roundedBoxRGBA(box.win->renderer,
                caretFinalX, caretFinalY + box.fontSize / 5,
                caretFinalX + 1, caretFinalY + box.fontSize / 5 + box.fontSize,
                0,
                box.clr.r, box.clr.g, box.clr.b, caretAlpha);
        }

        if (box.selectStart != -1 && box.selectEnd != -1 && box.selectStart != box.selectEnd) {
            int selMin = std::min(box.selectStart, box.selectEnd);
            int selMax = std::max(box.selectStart, box.selectEnd);

            int pos = selMin;
            while (pos < selMax) {
                size_t nlPos = box.buf.find('\n', pos);
                int lineEnd = (nlPos == std::string::npos || (int)nlPos > selMax) ? selMax : (int)nlPos;

                size_t curLineStart = box.buf.rfind('\n', pos > 0 ? pos - 1 : 0);
                curLineStart = (curLineStart == std::string::npos) ? 0 : curLineStart + 1;

                int lineIndex = (int)std::count(box.buf.begin(), box.buf.begin() + pos, '\n');

                std::string beforeStr = box.buf.substr(curLineStart, pos - curLineStart);
                std::string selStr = box.buf.substr(pos, lineEnd - pos);

                int wBefore = 0, wSel = 0, h = 0;
                if (!beforeStr.empty()) TTF_SizeUTF8(box.font, beforeStr.c_str(), &wBefore, &h);
                if (!selStr.empty())    TTF_SizeUTF8(box.font, selStr.c_str(), &wSel, &h);

                int selX = box.rect.x + offX + wBefore;
                int selY = box.rect.y + offY + lineIndex * lineSkip;
                int selH = h > 0 ? h : box.fontSize;

                roundedBoxRGBA(box.win->renderer,
                    selX, selY,
                    selX + wSel, selY + selH,
                    0,
                    80, 140, 255, 120);

                pos = lineEnd + 1; 
            }
        }

        SDL_RenderSetClipRect(box.win->renderer, nullptr);
    } else {
        Engine& eng = box.win->eng;
        Rect& rect = box.rect;

        if (box.debuggingBox) {
            eng.drawRoundedRectV(rect.x+offX, rect.y+offY, rect.w, rect.h, 0, {70, 70, 70, 255});
        }

        if (!box.buf.empty()) {
            eng.drawTextV(box.pathToFont.c_str(), box.buf.c_str(), box.fontSize,
                (float)(rect.x+offX), (float)(rect.y+offY),
                {box.clr.r, box.clr.g, box.clr.b, box.clr.a});
        } else if (!box.focus && !box.hint.empty()) {
            eng.drawTextV(box.pathToFont.c_str(), box.hint.c_str(), box.fontSize,
                (float)(rect.x+offX), (float)(rect.y+offY),
                {170, 170, 170, 170});
        }

        float lineSkip = box.fontSize * 1.2f; 

        if (box.focus) {
            float t = SDL_GetTicks() / 1000.0f;
            float phase = sinf(t * 3.14159f * 2.0f / 1.0f);
            uint8_t caretAlpha = (uint8_t)(((phase + 1.0f) / 2.0f) * box.clr.a);

            size_t lineStart = box.buf.rfind('\n', box.caret > 0 ? box.caret - 1 : 0);
            lineStart = (lineStart == std::string::npos) ? 0 : lineStart + 1;

            int lineIndex = (int)std::count(box.buf.begin(),
                                             box.buf.begin() + std::min((size_t)box.caret, box.buf.size()),
                                             '\n');

            std::string curLine = box.buf.substr(lineStart, box.caret - lineStart);

            float lineW = curLine.empty() ? 0.0f :
                eng.measureTextV(box.pathToFont.c_str(), curLine.c_str(), box.fontSize);

            float caretX = (float)(rect.x + offX) + lineW;
            float caretY = (float)(rect.y + offY) + lineIndex * lineSkip;

            eng.drawRoundedRectV(
                (int)caretX, (int)caretY + box.fontSize / 5, 2, box.fontSize, 0,
                {box.clr.r, box.clr.g, box.clr.b, caretAlpha});
        }

        if (box.selectStart != -1 && box.selectEnd != -1 && box.selectStart != box.selectEnd) {
            int selMin = std::min(box.selectStart, box.selectEnd);
            int selMax = std::max(box.selectStart, box.selectEnd);

            int pos = selMin;
            while (pos < selMax) {
                size_t nlPos = box.buf.find('\n', pos);
                int lineEnd = (nlPos == std::string::npos || (int)nlPos > selMax) ? selMax : (int)nlPos;

                size_t curLineStart = box.buf.rfind('\n', pos > 0 ? pos - 1 : 0);
                curLineStart = (curLineStart == std::string::npos) ? 0 : curLineStart + 1;

                int lineIndex = (int)std::count(box.buf.begin(), box.buf.begin() + pos, '\n');

                std::string beforeStr = box.buf.substr(curLineStart, pos - curLineStart);
                std::string selStr = box.buf.substr(pos, lineEnd - pos);

                float wBefore = beforeStr.empty() ? 0.0f :
                    eng.measureTextV(box.pathToFont.c_str(), beforeStr.c_str(), box.fontSize);
                float wSel = selStr.empty() ? 0.0f :
                    eng.measureTextV(box.pathToFont.c_str(), selStr.c_str(), box.fontSize);

                float selX = (float)(rect.x + offX) + wBefore;
                float selY = (float)(rect.y + offY) + lineIndex * lineSkip;

                eng.drawRoundedRectV(
                    (int)selX, (int)selY + box.fontSize / 5, (int)wSel, box.fontSize, 0,
                    {80, 140, 255, 120});

                pos = lineEnd + 1;
            }
        }
    }

    if (ownDraw) {
        box.ownDraw = true;
    } else {
        box.ownDraw = false;
    }
}

void drawSlider(Slider& s, bool ownDraw, int offX, int offY) {
    if (s.rect.w == 0 || s.rect.h == 0 || !s.Visible) return;

    if (!s.win->isVulkan) {
        auto makeTarget = [&](SDL_Renderer* renderer, int tw, int th) {
            SDL_Texture* t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, tw, th);
            SDL_SetTextureScaleMode(t, SDL_ScaleModeLinear);
            SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
            SDL_SetRenderTarget(renderer, t);
            SDL_SetRenderDrawColor(renderer, 0,0,0,0);
            SDL_RenderClear(renderer);
            return t;
        };

        SDL_Renderer* renderer = s.win->renderer;
        const int scale = 4;
        int knobR = (int)(s.rect.h*1.75f);
        int margin = knobR - s.rect.h/2 + 2; 

        int texW = s.rect.w + margin*2;
        int texH = s.rect.h + margin*2;

        SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);

        SDL_Texture* tex4x = makeTarget(renderer, texW*scale, texH*scale);
        int mx = margin*scale, my = margin*scale;
        int rw = s.rect.w*scale, rh = s.rect.h*scale;

        roundedBoxRGBA(renderer,
            mx, my, mx+rw, my+rh,
            rh/2, s.clrOff.r, s.clrOff.g, s.clrOff.b, s.clrOff.a);

        roundedBoxRGBA(renderer,
            mx, my, mx+(int)(rw*s.progress), my+rh,
            rh/2, s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);

        int cx = mx + (int)(rw*s.progress);
        int cy = my + rh/2;
        int rKnob = knobR*scale;
        filledCircleRGBA(renderer, cx, cy, rKnob, s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);
        aacircleRGBA    (renderer, cx, cy, rKnob, s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);

        SDL_Texture* tex2x = makeTarget(renderer, texW*2, texH*2);
        SDL_SetTextureBlendMode(tex4x, SDL_BLENDMODE_NONE);
        SDL_Rect dst2x = {0,0, texW*2, texH*2};
        SDL_RenderCopy(renderer, tex4x, nullptr, &dst2x);
        SDL_DestroyTexture(tex4x);

        SDL_Texture* tex1x = makeTarget(renderer, texW, texH);
        SDL_SetTextureBlendMode(tex2x, SDL_BLENDMODE_NONE);
        SDL_Rect dst1x = {0,0, texW, texH};
        SDL_RenderCopy(renderer, tex2x, nullptr, &dst1x);
        SDL_DestroyTexture(tex2x);

        SDL_SetRenderTarget(renderer, prevTarget);
        SDL_Rect dstFinal = {
            s.rect.x+offX-margin, s.rect.y+offY-margin,
            texW, texH
        };
        SDL_RenderCopy(renderer, tex1x, nullptr, &dstFinal);
        SDL_DestroyTexture(tex1x);
    } else {
        Engine& eng = s.win->eng;

        eng.drawRoundedRectV(s.rect.x+offX, s.rect.y+offY, s.rect.w, s.rect.h,
            s.rect.h/2, {s.clrOff.r, s.clrOff.g, s.clrOff.b, s.clrOff.a});

        eng.drawRoundedRectV(s.rect.x+offX, s.rect.y+offY,
            (int)(s.rect.w*s.progress), s.rect.h,
            s.rect.h/2, {s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a});

        int cx = s.rect.x+offX+(int)(s.rect.w*s.progress);
        int cy = s.rect.y+offY+s.rect.h/2;

        eng.drawCircleV((float)cx, (float)cy, (float)(s.rect.h*1.75f),
            {s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a});
    }

    if (ownDraw) {
        s.ownDraw = true;
    } else {
        s.ownDraw = false;
    }
}

void drawRangeSlider(RangeSlider& s, bool ownDraw, int offX, int offY) {
    auto makeTarget = [&](SDL_Renderer* renderer, int tw, int th) {
        SDL_Texture* t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, tw, th);
        SDL_SetTextureScaleMode(t, SDL_ScaleModeLinear);
        SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer, t);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        return t;
    };

    if (s.rect.w == 0 || s.rect.h == 0 || !s.Visible) return;

    if (!s.win->isVulkan) {
        SDL_Renderer* renderer = s.win->renderer;
        const int scale = 4;
        int knobR = (int)(s.rect.h*1.75f);
        int margin = knobR - s.rect.h/2 + 2;

        int texW = s.rect.w + margin*2;
        int texH = s.rect.h + margin*2;

        SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);

        SDL_Texture* tex4x = makeTarget(renderer, texW*scale, texH*scale);
        int mx = margin*scale, my = margin*scale;
        int rw = s.rect.w*scale, rh = s.rect.h*scale;

        roundedBoxRGBA(renderer,
            mx, my, mx+rw, my+rh,
            rh/2, s.clrOff.r, s.clrOff.g, s.clrOff.b, s.clrOff.a);

        roundedBoxRGBA(renderer,
            mx+(int)(rw*s.progress), my,
            mx+(int)(rw*s.progress2), my+rh,
            rh/2, s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);

        int rKnob = knobR*scale;
        for (float p : {s.progress, s.progress2}) {
            int cx = mx + (int)(rw*p);
            int cy = my + rh/2;
            filledCircleRGBA(renderer, cx, cy, rKnob, s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);
            aacircleRGBA    (renderer, cx, cy, rKnob, s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a);
        }

        SDL_Texture* tex2x = makeTarget(renderer, texW*2, texH*2);
        SDL_SetTextureBlendMode(tex4x, SDL_BLENDMODE_NONE);
        SDL_Rect dst2x = {0,0, texW*2, texH*2};
        SDL_RenderCopy(renderer, tex4x, nullptr, &dst2x);
        SDL_DestroyTexture(tex4x);

        SDL_Texture* tex1x = makeTarget(renderer, texW, texH);
        SDL_SetTextureBlendMode(tex2x, SDL_BLENDMODE_NONE);
        SDL_Rect dst1x = {0,0, texW, texH};
        SDL_RenderCopy(renderer, tex2x, nullptr, &dst1x);
        SDL_DestroyTexture(tex2x);

        SDL_SetRenderTarget(renderer, prevTarget);
        SDL_Rect dstFinal = {
            s.rect.x+offX-margin, s.rect.y+offY-margin,
            texW, texH
        };
        SDL_RenderCopy(renderer, tex1x, nullptr, &dstFinal);
        SDL_DestroyTexture(tex1x);
    } else {
        Engine& eng = s.win->eng;

        eng.drawRoundedRectV(s.rect.x+offX, s.rect.y+offY, s.rect.w, s.rect.h,
                s.rect.h/2, {s.clrOff.r, s.clrOff.g, s.clrOff.b, s.clrOff.a});

        eng.drawRoundedRectV(s.rect.x+offX+(int)(s.rect.w*s.progress), s.rect.y+offY,
            (int)(s.rect.w*(s.progress2-s.progress)), s.rect.h,
            s.rect.h/2, {s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a});

        for (float p : {s.progress, s.progress2}) {
            int cx = s.rect.x+offX+(int)(s.rect.w*p);
            int cy = s.rect.y+offY+s.rect.h/2;
            eng.drawCircleV((float)cx, (float)cy, (float)(s.rect.h*1.75f),
                {s.clrOn.r, s.clrOn.g, s.clrOn.b, s.clrOn.a});
        }
    }

    if (ownDraw) {
        s.ownDraw = true;
    } else {
        s.ownDraw = false;
    }
}

void redrawAllOthObjs();

void renderSceneAndCapture(Window* win, int captureX, int captureY, int w, int h, std::vector<Color>& outPixels, uint32_t& imageIndexOut) {
    win->eng.renderSceneAndCapture(captureX, captureY, w, h, outPixels, imageIndexOut);
}

void renderOverlayAndPresent(Window* win, uint32_t imageIndex) {
    win->eng.renderOverlayAndPresent(imageIndex);
}

void markCanvasForCapture(Window* win, const char* canvasName, int srcX, int srcY) {
    win->eng.markCanvasForCapture(canvasName, srcX, srcY);
}

void unmarkCanvasForCapture(Window* win, const char* canvasName) {
    win->eng.unmarkCanvasForCapture(canvasName);
}

void UpdateCheckBoxButton(CheckBox& box, bool ownDraw, SDL_Renderer* renderer) {
    if (box.uRect.size == 0 || !box.Visible) return;
    
    if (!box.win->isVulkan) {
        auto ensureCheckboxAATextures = [&](CheckBox& box, SDL_Renderer* renderer, int size) {
            if (box.aaTexSize == size && box.aaTex4x) return;

            auto make = [&](int s) {
                SDL_Texture* t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, s, s);
                SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
                SDL_SetTextureScaleMode(t, SDL_ScaleModeLinear);
                return t;
            };

            if (box.aaTex4x) SDL_DestroyTexture(box.aaTex4x);
            if (box.aaTex2x) SDL_DestroyTexture(box.aaTex2x);
            if (box.aaTex1x) SDL_DestroyTexture(box.aaTex1x);

            box.aaTex4x = make(size*4);
            box.aaTex2x = make(size*2);
            box.aaTex1x = make(size);
            box.aaTexSize = size;
        };

        auto& u = box.uRect;
        ensureCheckboxAATextures(box, renderer, u.size);
        const int scale = 4;

        SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);

        SDL_SetRenderTarget(renderer, box.aaTex4x);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);

        if (box.isAnim) {
            box.animProgress += mainSpace.deltaTime / box.animDuration;
            if (box.animProgress >= 1.0f) {
                box.animProgress = 0.0f;
                box.isAnim = false;
            }
            redrawAllOthObjs();

            roundedBoxRGBA(renderer, 0, 0, u.size*scale, u.size*scale,
                (u.size/6)*scale, box.clr.r, box.clr.g, box.clr.b, box.clr.a);

            int pad = (int)(u.size*0.07f) * scale;
            roundedBoxRGBA(renderer, pad, pad, u.size*scale-pad, u.size*scale-pad,
                (u.size/6)*scale, 255,255,255,255);

            float t = box.isOn ? box.animProgress : (1.0f - box.animProgress);
            float cs = u.size * t;
            float cx = (u.size-cs)/2.f;
            float cy = (u.size-cs)/2.f;

            if (cs > 0.5f) {
                roundedBoxRGBA(renderer, (int)(cx*scale), (int)(cy*scale),
                    (int)((cx+cs)*scale), (int)((cy+cs)*scale),
                    (int)(cs/6)*scale, box.clr.r, box.clr.g, box.clr.b, box.clr.a);
            }
        } else {
            roundedBoxRGBA(renderer, 0, 0, u.size*scale, u.size*scale,
                (u.size/6)*scale, box.clr.r, box.clr.g, box.clr.b, box.clr.a);

            int pad = (int)(u.size*0.07f) * scale;
            roundedBoxRGBA(renderer, pad, pad, u.size*scale-pad, u.size*scale-pad,
                (u.size/6)*scale, 255,255,255,255);

            if (box.isOn)
                roundedBoxRGBA(renderer, 0, 0, u.size*scale, u.size*scale,
                    (u.size/6)*scale, box.clr.r, box.clr.g, box.clr.b, box.clr.a);
        }

        SDL_SetRenderTarget(renderer, box.aaTex2x);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_SetTextureBlendMode(box.aaTex4x, SDL_BLENDMODE_NONE);
        SDL_Rect dst2x = {0,0, u.size*2, u.size*2};
        SDL_RenderCopy(renderer, box.aaTex4x, nullptr, &dst2x);
        SDL_SetTextureBlendMode(box.aaTex4x, SDL_BLENDMODE_BLEND); 

        SDL_SetRenderTarget(renderer, box.aaTex1x);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_SetTextureBlendMode(box.aaTex2x, SDL_BLENDMODE_NONE);
        SDL_Rect dst1x = {0,0, u.size, u.size};
        SDL_RenderCopy(renderer, box.aaTex2x, nullptr, &dst1x);
        SDL_SetTextureBlendMode(box.aaTex2x, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(renderer, prevTarget);
        SDL_Rect dstFinal = {u.x, u.y, u.size, u.size};
        SDL_RenderCopy(renderer, box.aaTex1x, nullptr, &dstFinal);
    } else {
        Engine& eng = box.win->eng;
        auto& u = box.uRect;

        if (box.isAnim) {
            box.animProgress += mainSpace.deltaTime / box.animDuration;
            
            if (box.animProgress >= 1.0f) {
                box.animProgress = 0.0f;
                box.isAnim = false;
            }

            eng.drawRoundedRectV(u.x, u.y, u.size, u.size,
                u.size/6, {box.clr.r, box.clr.g, box.clr.b, box.clr.a});

            int pad = (int)(u.size*0.07f);
            eng.drawRoundedRectV(u.x+pad, u.y+pad, u.size-2*pad, u.size-2*pad,
                u.size/6, {255,255,255,255});

            float t = box.isOn ? box.animProgress : (1.0f - box.animProgress);
            float cs = u.size * t;
            float cx = u.x + (u.size-cs)/2.f;
            float cy = u.y + (u.size-cs)/2.f;

            if (cs > 0.5f) {
                eng.drawRoundedRectV((int)cx, (int)cy, (int)cs, (int)cs,
                    (int)(cs/6), {box.clr.r, box.clr.g, box.clr.b, box.clr.a});
            }
        } else {
            eng.drawRoundedRectV(u.x, u.y, u.size, u.size,
                u.size/6, {box.clr.r, box.clr.g, box.clr.b, box.clr.a});

            int pad = (int)(u.size*0.07f);
            eng.drawRoundedRectV(u.x+pad, u.y+pad, u.size-2*pad, u.size-2*pad,
                u.size/6, {255,255,255,255});

            if (box.isOn)
                eng.drawRoundedRectV(u.x, u.y, u.size, u.size,
                    u.size/6, {box.clr.r, box.clr.g, box.clr.b, box.clr.a});
        }
    }

    if (ownDraw) {
        box.ownDraw = true;
    } else {
        box.ownDraw = false;
    }
}

void UpdateRadioButton(RadioButton& radio, bool ownDraw) {
    if (radio.uRect.size == 0 || !radio.Visible) return;

    if (!radio.win->isVulkan) {
        SDL_Renderer* renderer = radio.win->renderer;
        auto& u   = radio.uRect;
        auto& cOn = radio.clrOn;
        auto& cOf = radio.clrOff;
        bool  st  = radio.status;

        if (radio.animPending) {
            radio.animProgress += mainSpace.deltaTime / radio.animDuration;
            if (radio.animProgress >= 1.0f) {
                radio.animProgress = 0.0f;
                radio.animPending  = false;
            }

            float t = st ? radio.animProgress : (1.0f - radio.animProgress);

            filledCircleRGBA(renderer, u.x, u.y, u.size, cOn.r,cOn.g,cOn.b,cOn.a);
            aacircleRGBA    (renderer, u.x, u.y, u.size, cOn.r,cOn.g,cOn.b,cOn.a);
            filledCircleRGBA(renderer, u.x, u.y, (int)(u.size-u.size*0.2f), 255,255,255,255);
            aacircleRGBA    (renderer, u.x, u.y, (int)(u.size-u.size*0.2f), 255,255,255,255);
            filledCircleRGBA(renderer, u.x, u.y, (int)(u.size/1.525f*t), cOn.r,cOn.g,cOn.b,cOn.a);
            aacircleRGBA    (renderer, u.x, u.y, (int)(u.size/1.525f*t), cOn.r,cOn.g,cOn.b,cOn.a);
        } else {
            if (st) {
                filledCircleRGBA(renderer, u.x,u.y,u.size, cOn.r,cOn.g,cOn.b,cOn.a);
                aacircleRGBA    (renderer, u.x,u.y,u.size, cOn.r,cOn.g,cOn.b,cOn.a);
                filledCircleRGBA(renderer, u.x,u.y,(int)(u.size-u.size*0.2f),255,255,255,255);
                aacircleRGBA    (renderer, u.x,u.y,(int)(u.size-u.size*0.2f),255,255,255,255);
                filledCircleRGBA(renderer, u.x,u.y,(int)(u.size/1.525f),cOn.r,cOn.g,cOn.b,cOn.a);
                aacircleRGBA    (renderer, u.x,u.y,(int)(u.size/1.525f),cOn.r,cOn.g,cOn.b,cOn.a);
            } else {
                filledCircleRGBA(renderer, u.x,u.y,u.size, cOf.r,cOf.g,cOf.b,cOf.a);
                aacircleRGBA    (renderer, u.x,u.y,u.size, cOf.r,cOf.g,cOf.b,cOf.a);
                filledCircleRGBA(renderer, u.x,u.y,(int)(u.size-u.size*0.2f),255,255,255,255);
                aacircleRGBA    (renderer, u.x,u.y,(int)(u.size-u.size*0.2f),255,255,255,255);
            }
        }
    } else {
        Engine& eng = radio.win->eng;
        auto& u   = radio.uRect;
        auto& cOn = radio.clrOn;
        auto& cOf = radio.clrOff;
        bool  st  = radio.status;

        if (radio.animPending) {
            radio.animProgress += mainSpace.deltaTime / radio.animDuration;
            if (radio.animProgress >= 1.0f) {
                radio.animProgress = 0.0f;
                radio.animPending  = false;
            }

            float t = st ? radio.animProgress : (1.0f - radio.animProgress);

            eng.drawCircleV((float)u.x, (float)u.y, (float)u.size, cOn);
            eng.drawCircleV((float)u.x, (float)u.y, (float)(u.size-u.size*0.2f), {255,255,255,255});
            eng.drawCircleV((float)u.x, (float)u.y, (float)(u.size/1.525f*t), cOn);
        } else {
            if (st) {
                eng.drawCircleV((float)u.x, (float)u.y, (float)u.size, cOn);
                eng.drawCircleV((float)u.x, (float)u.y, (float)(u.size-u.size*0.2f), {255,255,255,255});
                eng.drawCircleV((float)u.x, (float)u.y, (float)(u.size/1.525f), cOn);
            } else {
                eng.drawCircleV((float)u.x, (float)u.y, (float)u.size, cOf);
                eng.drawCircleV((float)u.x, (float)u.y, (float)(u.size-u.size*0.2f), {255,255,255,255});
            }
        }
    }

    if (ownDraw) {
        radio.ownDraw = true;
    } else {
        radio.ownDraw = false;
    }
}

void drawElement(UIElement& el, bool ownDraw, int offX, int offY) {
    std::visit([&](auto& e) {
        using T = std::decay_t<decltype(e)>;
        if      constexpr(std::is_same_v<T,Box>)         { drawBox(e, ownDraw, offX, offY); }
        else if constexpr(std::is_same_v<T,Circle>)      { drawCircle(e, ownDraw, offX, offY); }
        else if constexpr(std::is_same_v<T,Text>)        { drawText(e, ownDraw, offX, offY); }
        else if constexpr(std::is_same_v<T,Image>)       { drawImage(e, ownDraw, offX, offY); }
        else if constexpr(std::is_same_v<T,InputBox>)    { drawInputBox(e, ownDraw, offX, offY); }
        else if constexpr(std::is_same_v<T,CheckBox>)    { UpdateCheckBoxButton(e, ownDraw, e.win->renderer); }
        else if constexpr(std::is_same_v<T,RadioButton>) { if (e.win != nullptr) UpdateRadioButton(e, ownDraw); }
        else if constexpr(std::is_same_v<T,Line>)        { drawLine(e, ownDraw, offX, offY); }
        else if constexpr(std::is_same_v<T,Slider>)      { drawSlider(e, ownDraw, offX, offY); }
        else if constexpr(std::is_same_v<T,RangeSlider>) { drawRangeSlider(e, ownDraw, offX, offY); }
    }, el);
    
    if (ownDraw) {
        std::visit([&](auto& obj) {
            obj.ownDraw = true;
        }, el);
    } else {
        std::visit([&](auto& obj) {
            obj.ownDraw = false;
        }, el);
    }
}

void redrawAllOthObjs() {
    for (auto& win : mainSpace.wins) {
        bool hasBg = false;
        for (auto& bg : mainSpace.backgrounds) {
            if (std::get<1>(bg)->renderer == win->renderer) {
                SDL_SetRenderDrawColor(win->renderer,
                    std::get<0>(bg).r, std::get<0>(bg).g, std::get<0>(bg).b, std::get<0>(bg).a);
                hasBg = true; break;
            }
        }
        if (!hasBg) SDL_SetRenderDrawColor(win->renderer, 100,100,100,255);
        if (win->renderer != nullptr) SDL_RenderClear(win->renderer);
    }

    for (auto& el : mainSpace.uiElements) {
        if (std::holds_alternative<ScrollBar>(el)) { continue; }

        const std::string& ename = std::visit(GetName{}, el);

        if (isInScrollBar(ename)) continue;

        bool skip = std::visit([](auto& e){
            using T = std::decay_t<decltype(e)>;
            if constexpr(std::is_same_v<T,CheckBox>)    return e.isAnim;
            if constexpr(std::is_same_v<T,RadioButton>) return e.animPending;
            if constexpr(std::is_same_v<T,ScrollBar>)   return true;
            return false;
        }, el);

        if (!skip) drawElement(el);
    }
}

void UpdateAll() {
    redrawAllOthObjs();

    for (auto& el : mainSpace.uiElements) {
        if (std::holds_alternative<ScrollBar>(el)) continue;

        const std::string& ename = std::visit(GetName{}, el);

        if (isInScrollBar(ename)) continue;

        bool forceDraw = std::visit([](auto& e){
            using T = std::decay_t<decltype(e)>;
            if constexpr(std::is_same_v<T,CheckBox>)    return e.isAnim;
            if constexpr(std::is_same_v<T,RadioButton>) return e.animPending;
            return false;
        }, el);

        bool ownDraw;

        std::visit([&](auto& obj) {
            ownDraw = obj.ownDraw;
        }, el);

        if (!ownDraw || forceDraw) drawElement(el, false);
    }

    for (auto& win : mainSpace.wins) {
        if (win->eng.started && !win->eng.cleaned)
            win->eng.flush();
    }

    for (auto& el : mainSpace.uiElements) {
        if (!std::holds_alternative<ScrollBar>(el)) continue;
        ScrollBar& sb = std::get<ScrollBar>(el);

        if (sb.rect.w == 0 || sb.rect.h == 0 || !sb.Visible) { continue; }

        if (!sb.win->isVulkan) {
            auto makeTarget = [&](SDL_Renderer* renderer, int tw, int th) {
                SDL_Texture* t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, tw, th);
                SDL_SetTextureScaleMode(t, SDL_ScaleModeLinear);
                SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
                SDL_SetRenderTarget(renderer, t);
                SDL_SetRenderDrawColor(renderer, 0,0,0,0);
                SDL_RenderClear(renderer);
                return t;
            };

            SDL_Rect clip = {sb.rect.x, sb.rect.y, sb.rect.w, sb.rect.h};
            SDL_RenderSetClipRect(sb.win->renderer, &clip);

            SDL_Renderer* renderer = sb.win->renderer;
            int w = sb.rect.w, h = sb.rect.h;
            const int scale = 4;

            SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);

            SDL_Texture* tex4x = makeTarget(renderer, w*scale, h*scale);

            if (sb.debuggingBox) {
                SDL_Rect clip4x = {0, 0, w*scale, h*scale};
                SDL_RenderSetClipRect(renderer, &clip4x);

                roundedBoxRGBA(renderer,
                    0, 0, w*scale, h*scale,
                    sb.rad*scale, sb.color.r, sb.color.g, sb.color.b, sb.color.a);
            }

            float contentH = 0.0f;
            for (auto& childName : sb.elements) {
                for (auto& cel : mainSpace.uiElements) {
                    if (std::visit(GetName{}, cel) != childName) continue;
                    std::visit([&](auto& e) {
                        using T = std::decay_t<decltype(e)>;
                        if constexpr (requires { e.rect; })
                            contentH = std::max(contentH, (float)(e.rect.y + e.rect.h));
                        else if constexpr (requires { e.uRect; })
                            contentH = std::max(contentH, (float)(e.uRect.y + e.uRect.size));
                    }, cel);
                    break;
                }
            }

            float sliderH = (sb.rect.h / std::sqrt((float)std::max((size_t)1, sb.elements.size()))) * sb.scrollingSize;
            float scrollableH = (contentH - sb.rect.h) * sb.scrollingSize;
            int scrollOffY = (int)(sb.pos * -scrollableH);
            float trackH  = h - sliderH;
            float thumbTop = sb.pos * trackH; 

            if (sb.rect.h != sliderH) {
                roundedBoxRGBA(renderer,
                    (int)(w*0.95f/sb.sliderSize*scale), (int)(thumbTop*scale),
                    w*scale,              (int)((thumbTop+sliderH)*scale),
                    (w/21)*scale, 70,70,70,255);
            }

            SDL_RenderSetClipRect(renderer, nullptr); 

            SDL_Texture* tex2x = makeTarget(renderer, w*2, h*2);
            SDL_SetTextureBlendMode(tex4x, SDL_BLENDMODE_NONE);
            SDL_Rect dst2x = {0,0, w*2, h*2};
            SDL_RenderCopy(renderer, tex4x, nullptr, &dst2x);
            SDL_DestroyTexture(tex4x);

            SDL_Texture* tex1x = makeTarget(renderer, w, h);
            SDL_SetTextureBlendMode(tex2x, SDL_BLENDMODE_NONE);
            SDL_Rect dst1x = {0,0, w, h};
            SDL_RenderCopy(renderer, tex2x, nullptr, &dst1x);
            SDL_DestroyTexture(tex2x);

            SDL_SetRenderTarget(renderer, prevTarget);
            SDL_Rect dstFinal = {sb.rect.x, sb.rect.y, w, h};
            SDL_RenderCopy(renderer, tex1x, nullptr, &dstFinal);
            SDL_DestroyTexture(tex1x);

            for (auto& childName : sb.elements) {
                for (auto& cel : mainSpace.uiElements) {
                    if (std::visit(GetName{}, cel) != childName) continue;
                    if (std::holds_alternative<ScrollBar>(cel)) continue;
                    
                    drawElement(cel, false, sb.rect.x, sb.rect.y - scrollOffY);
                    break;
                }
            }

            SDL_RenderSetClipRect(sb.win->renderer, nullptr);
        } else {
            Engine& eng = sb.win->eng;

            eng.setScissorV(sb.rect.x, sb.rect.y, sb.rect.w, sb.rect.h);

            if (sb.debuggingBox) {
                eng.drawRoundedRectV(sb.rect.x, sb.rect.y, sb.rect.w, sb.rect.h,
                    sb.rad, {sb.color.r, sb.color.g, sb.color.b, sb.color.a});
            }

            float contentH = 0.0f;
            for (auto& childName : sb.elements) {
                for (auto& cel : mainSpace.uiElements) {
                    if (std::visit(GetName{}, cel) != childName) continue;
                    std::visit([&](auto& e) {
                        using T = std::decay_t<decltype(e)>;
                        if constexpr (requires { e.rect; })
                            contentH = std::max(contentH, (float)(e.rect.y + e.rect.h));
                        else if constexpr (requires { e.uRect; })
                            contentH = std::max(contentH, (float)(e.uRect.y + e.uRect.size));
                    }, cel);
                    break;
                }
            }

            float sliderH = (sb.rect.h / std::sqrt((float)std::max((size_t)1, sb.elements.size()))) * sb.scrollingSize;

            float trackH  = sb.rect.h - sliderH;
            float thumbTop = sb.rect.y + sb.pos * trackH;

            if (sb.rect.h != sliderH) {
                eng.drawRoundedRectV(
                    sb.rect.x + (int)(sb.rect.w*0.95f/sb.sliderSize), (int)thumbTop,
                    sb.rect.w - (int)(sb.rect.w*0.95f/sb.sliderSize), (int)sliderH,
                    sb.rect.w/21, {70, 70, 70, 255});
            }

            float scrollableH = (contentH - sb.rect.h) * sb.scrollingSize;
            int scrollOffY = (int)(sb.pos * -scrollableH);

            for (auto& childName : sb.elements) {
                for (auto& cel : mainSpace.uiElements) {
                    if (std::visit(GetName{}, cel) != childName) continue;
                    if (std::holds_alternative<ScrollBar>(cel)) continue;
                    drawElement(cel, false, sb.rect.x,sb.rect.y - scrollOffY);
                    break;
                }
            }
            
            eng.flush(true, sb.rect.x, sb.rect.y, sb.rect.w, sb.rect.h);
            eng.resetScissorV();
        }
    }
}

Window* createWindow(const char* title, Rect rect, int windowFlags, int rendererFlags, std::string name) {
    if ((rendererFlags == RENDERER_VULKAN || rendererFlags == RENDERER_OPTIMAL) && windowFlags != WINDOW_VULKAN)
        windowFlags |= WINDOW_VULKAN;

    bool wantVulkan = (rendererFlags & RENDERER_VULKAN) != 0;

    if (!wantVulkan) {
        windowFlags |= SDL_WINDOW_OPENGL;
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    }

    SDL_Window* wind = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, windowFlags);
    if (!wind) { fprintf(stderr, "CreateWindow: %s\n", SDL_GetError()); return nullptr; }

    Window* win = new Window{wind, nullptr, nullptr, name, {rect.x, rect.y, rect.w, rect.h}};

    if (wantVulkan) {
        win->isVulkan = true;
        win->eng.startWin = wind;
        win->eng.start();
    } else {
        SDL_Surface* surface = SDL_CreateRGBSurface(0, win->rect.w, win->rect.h, 0, 0, 0, 0, 0);

        win->surface = surface;
        win->renderer = SDL_CreateRenderer(wind, -1, rendererFlags);

        SDL_GL_MakeCurrent(wind, SDL_GL_GetCurrentContext());
        glEnable(GL_MULTISAMPLE);

        if (!win->renderer) {
            fprintf(stderr, "CreateRenderer: %s\n", SDL_GetError());
            SDL_DestroyWindow(wind);
            delete win;
            return nullptr;
        }
    }

    mainSpace.wins.push_back(win);
    mainSpace.names.push_back(name);
    return win;
}

Renderer* createRenderer(int rendererFlags, Window* win, std::string name) {
    Renderer* rnder = nullptr;

    bool wantVulkan = (rendererFlags & RENDERER_VULKAN) != 0;
    
    if (!wantVulkan) {
        SDL_Window* wind = win->win;

        SDL_Renderer* rnd = SDL_CreateRenderer(wind, -1, rendererFlags);
        
        if (!rnd) { 
            fprintf(stderr,"CreateRenderer: %s\n",SDL_GetError()); 
            SDL_DestroyRenderer(rnd); 

            return nullptr; 
        }
        
        rnder = new Renderer();
        rnder->renderer = rnd;
        mainSpace.renderers.push_back(rnder);
        mainSpace.names.push_back(name);
    } else {
        win->isVulkan = true;
        win->eng.startWin = win->win;
        win->eng.start();
    }
    
    return rnder;
}

Box& createBox(Window* win, Rect rect, Color clr, std::string name) {
    mainSpace.uiElements.emplace_back(Box{rect, clr, name, win});
    mainSpace.names.push_back(name);
    return std::get<Box>(mainSpace.uiElements.back());
}

Circle& createCircle(Window* win, Square rect, Color clr, std::string name) {
    mainSpace.uiElements.emplace_back(Circle{rect, clr, name, win});
    mainSpace.names.push_back(name);
    return std::get<Circle>(mainSpace.uiElements.back());
}

Text& createText(Window* win, std::string path, std::string text, Rect rect, Color clr, int fontSize, std::string name) {
    if (win->isVulkan) {
        win->eng.acquireFontRef(resolveFontPath(path), fontSize);
        
        mainSpace.uiElements.emplace_back(
            Text(rect, clr, path, fontSize, text, name, win, nullptr, nullptr, nullptr));
        mainSpace.names.push_back(name);
        return std::get<Text>(mainSpace.uiElements.back());
    }

    std::string fp = resolveFontPath(path);
    TTF_Font* font = TTF_OpenFont(fp.c_str(), fontSize);
    
    if (!font) { std::cerr<<"ERROR: Cannot find font\n"; mainSpace.running=false; static Text dummy; return dummy; }
    
    SDL_Surface* surf = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), {clr.r,clr.g,clr.b,clr.a}, 0);
    SDL_Texture* tex  = SDL_CreateTextureFromSurface(win->renderer, surf);
    mainSpace.uiElements.emplace_back(
        Text(rect,clr,path,fontSize,text,name,win,font,surf,tex));
    mainSpace.names.push_back(name);
    
    return std::get<Text>(mainSpace.uiElements.back());
}

Image& createImage(Window* win, std::string path, Rect rect, std::string name) {
    std::ifstream img(path.c_str());
    if (!img.good()) {
        std::cout << "ERROR: Invalid path to the image";
        mainSpace.running = false;
        static Image dummy;
        return dummy;
    }

    if (win->isVulkan) {
        win->eng.acquireImgRef(path);
        mainSpace.uiElements.emplace_back(Image(rect, path, nullptr, name, win));
        mainSpace.names.push_back(name);
        return std::get<Image>(mainSpace.uiElements.back());
    }

    SDL_Texture* tex = IMG_LoadTexture(win->renderer, path.c_str());
    mainSpace.uiElements.emplace_back(Image(rect,path,tex,name,win));
    mainSpace.names.push_back(name);
    return std::get<Image>(mainSpace.uiElements.back());
}

InputBox& createInputBox(Window* win, std::string hint, Rect rect, std::string text, int fontSize, Color clr, std::string name, bool debuggingBox) {
    std::string path = "NotoSans.ttf";

    InputBox box{};
    box.rect=rect; box.clr=clr; box.name=name; box.win=win;
    box.buf=text; box.hint=hint; box.text=text;
    box.debuggingBox=debuggingBox;
    box.pathToFont=path; box.fontSize=fontSize;

    if (win->isVulkan) {
        win->eng.acquireFontRef(path, fontSize);
        box.font = nullptr;
        mainSpace.uiElements.emplace_back(std::move(box));
        mainSpace.names.push_back(name);
        return std::get<InputBox>(mainSpace.uiElements.back());
    }

    std::string fp = resolveFontPath(path);
    TTF_Font* font = TTF_OpenFont(fp.c_str(), fontSize);
    
    if (!font) { std::cerr<<"ERROR: Cannot find font\n"; mainSpace.running=false; static InputBox dummy; return dummy; }
    
    box.font = font;
    mainSpace.uiElements.emplace_back(std::move(box));
    mainSpace.names.push_back(name);
    
    return std::get<InputBox>(mainSpace.uiElements.back());
}

CheckBox& createCheckBox(Window* win, Square rect, Color clr, bool isOn, std::string name) {
    mainSpace.uiElements.emplace_back(CheckBox{rect,clr,name,isOn,false,win});
    mainSpace.names.push_back(name);
    
    return std::get<CheckBox>(mainSpace.uiElements.back());
}

RadioButton& createRadioButton(Window* win, Square uRect, Color clrOff, Color clrOn, bool status, int group, std::string name) {
    if (status) {
        for (auto& el : mainSpace.uiElements) {
            if (!std::holds_alternative<RadioButton>(el)) continue;
            RadioButton& r = std::get<RadioButton>(el);
            if (r.group == group) r.status = false;
        }
    }

    mainSpace.uiElements.emplace_back(RadioButton{uRect,status,false,clrOff,clrOn,group,win,name});
    mainSpace.names.push_back(name);

    return std::get<RadioButton>(mainSpace.uiElements.back());
}

Line& createLine(Window* win, Rect cords, Color color, std::string name) {
    mainSpace.uiElements.emplace_back(Line{cords,color,name,win});
    mainSpace.names.push_back(name);

    return std::get<Line>(mainSpace.uiElements.back());
}

Slider& createSlider(Window* win, Rect rect, Color clrOff, Color clrOn, float progress, std::string name) {
    mainSpace.uiElements.emplace_back(Slider{rect,clrOff,clrOn,progress,name,win});
    mainSpace.names.push_back(name);

    return std::get<Slider>(mainSpace.uiElements.back());
}

RangeSlider& createRangeSlider(Window* win, Rect rect, Color clrOff, Color clrOn, float progress, float progress2, std::string name) {
    mainSpace.uiElements.emplace_back(RangeSlider{rect,clrOff,clrOn,progress,progress2,name,win});
    mainSpace.names.push_back(name);

    return std::get<RangeSlider>(mainSpace.uiElements.back());
}

ScrollBar& createScrollBar(Window* win, Rect rect, Color clr, std::string name, bool debuggingBox) {
    mainSpace.uiElements.emplace_back(ScrollBar{rect,clr,name,win,debuggingBox});
    mainSpace.names.push_back(name);

    return std::get<ScrollBar>(mainSpace.uiElements.back());
}

void addScrollBarElement(Window*, std::string objName, std::string scrollBarName) {
    for (auto& el : mainSpace.uiElements) {
        if (!std::holds_alternative<ScrollBar>(el)) continue;
        ScrollBar& sb = std::get<ScrollBar>(el);
        if (sb.name == scrollBarName) { sb.elements.push_back(objName); return; }
    }

    SDL_Log("Scrollbar not found");
}

Sound createSound(const char* path, std::string name, int soundFlags) {
    Sound s{}; s.name = name;
    ma_sound_init_from_file(&mainSpace.engine, path, soundFlags, nullptr, nullptr, &s.sound);
    mainSpace.sounds.push_back(s);

    return mainSpace.sounds.back();
}

inline bool open_video(const char* path, DecoderState& st, bool isVulkan) {
    if (avformat_open_input(&st.fmt_ctx, path, nullptr, nullptr) < 0) {
        fprintf(stderr, "Failed to open the file: %s\n", path);
        return false;
    }

    if (avformat_find_stream_info(st.fmt_ctx, nullptr) < 0) return false;

    st.video_stream_idx = av_find_best_stream(st.fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (st.video_stream_idx < 0) return false;

    AVCodecParameters* codecpar = st.fmt_ctx->streams[st.video_stream_idx]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec) return false;

    st.codec_ctx = avcodec_alloc_context3(codec);
    if (avcodec_parameters_to_context(st.codec_ctx, codecpar) < 0) return false;
    if (avcodec_open2(st.codec_ctx, codec, nullptr) < 0) return false;

    if (isVulkan) {
        st.sws_ctx = sws_getContext(
            st.codec_ctx->width, st.codec_ctx->height, st.codec_ctx->pix_fmt,
            st.codec_ctx->width, st.codec_ctx->height, AV_PIX_FMT_RGBA,
            SWS_BILINEAR, nullptr, nullptr, nullptr);
        return st.sws_ctx != nullptr;
    } else {
        st.sws_ctx = sws_getContext(
            st.codec_ctx->width, st.codec_ctx->height, st.codec_ctx->pix_fmt,
            st.codec_ctx->width, st.codec_ctx->height, AV_PIX_FMT_YUV420P,
            SWS_BILINEAR, nullptr, nullptr, nullptr);
        return st.sws_ctx != nullptr;
    }
}

inline void open_audio(DecoderState& st) {
    st.audio_stream_idx = av_find_best_stream(st.fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (st.audio_stream_idx < 0) return;

    AVCodecParameters* codecpar = st.fmt_ctx->streams[st.audio_stream_idx]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec) { st.audio_stream_idx = -1; return; }

    st.audio_codec_ctx = avcodec_alloc_context3(codec);
    if (avcodec_parameters_to_context(st.audio_codec_ctx, codecpar) < 0 ||
        avcodec_open2(st.audio_codec_ctx, codec, nullptr) < 0) {
        st.audio_stream_idx = -1;
        return;
    }

    AVChannelLayout out_layout;
    av_channel_layout_default(&out_layout, st.out_channels);
    int ret = swr_alloc_set_opts2(&st.swr_ctx,
        &out_layout, AV_SAMPLE_FMT_S16, st.out_sample_rate,
        &st.audio_codec_ctx->ch_layout, st.audio_codec_ctx->sample_fmt, st.audio_codec_ctx->sample_rate,
        0, nullptr);
    av_channel_layout_uninit(&out_layout);

    if (ret < 0 || !st.swr_ctx || swr_init(st.swr_ctx) < 0) {
        avcodec_free_context(&st.audio_codec_ctx);
        st.audio_stream_idx = -1;
    }
}

inline AVFrame* alloc_yuv420p_frame(int w, int h) {
    AVFrame* f = av_frame_alloc();
    f->format = AV_PIX_FMT_YUV420P;
    f->width = w;
    f->height = h;
    if (av_frame_get_buffer(f, 32) < 0) {
        av_frame_free(&f);
        return nullptr;
    }
    return f;
}

inline AVFrame* alloc_rgba_frame(int w, int h) {
    AVFrame* f = av_frame_alloc();
    f->format = AV_PIX_FMT_RGBA;
    f->width = w;
    f->height = h;
    if (av_frame_get_buffer(f, 1) < 0) {
        av_frame_free(&f);
        return nullptr;
    }
    return f;
}

inline void handle_audio_packet(DecoderState* st, AVPacket* pkt,
                                 SDL_AudioDeviceID audio_device,
                                 AVFrame* audio_frame, uint8_t** resampled_buf, float volume) {
    if (avcodec_send_packet(st->audio_codec_ctx, pkt) < 0) return;

    while (true) {
        int ret = avcodec_receive_frame(st->audio_codec_ctx, audio_frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
        if (ret < 0) break;

        int out_samples = swr_get_out_samples(st->swr_ctx, audio_frame->nb_samples);
        int buf_size = av_samples_get_buffer_size(nullptr, st->out_channels, out_samples,
                                                    AV_SAMPLE_FMT_S16, 0);
        if (buf_size < 0) continue;

        av_freep(resampled_buf);
        av_samples_alloc(resampled_buf, nullptr, st->out_channels, out_samples,
                          AV_SAMPLE_FMT_S16, 0);

        int converted = swr_convert(st->swr_ctx, resampled_buf, out_samples,
                                     (const uint8_t**)audio_frame->data, audio_frame->nb_samples);
        if (converted > 0 && audio_device) {
            int bytes = converted * st->out_channels * (int)sizeof(int16_t);

            float vol = volume; 
            if (vol != 1.0f) {
                int16_t* samples = (int16_t*)*resampled_buf;
                int sample_count = bytes / (int)sizeof(int16_t);
                for (int i = 0; i < sample_count; ++i) {
                    int32_t scaled = (int32_t)(samples[i] * vol);
                    if (scaled > 32767) scaled = 32767;
                    if (scaled < -32768) scaled = -32768;
                    samples[i] = (int16_t)scaled;
                }
            }

            SDL_QueueAudio(audio_device, *resampled_buf, bytes);
        }
    }
}

inline void decoder_thread_func(VideoDecoderContext* ctx) {
    DecoderState* st = &ctx->decoderState;
    AVPacket* pkt = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    AVFrame* audio_frame = av_frame_alloc();
    uint8_t* resampled_buf = nullptr;

    double time_base = av_q2d(st->fmt_ctx->streams[st->video_stream_idx]->time_base);
    int width = st->codec_ctx->width;
    int height = st->codec_ctx->height;

    bool got_first_pts = false;
    int64_t first_pts = 0;

    while (ctx->decoding.load()) {
        if (ctx->seek_requested.exchange(false)) {
            double target = ctx->seek_target_seconds.load();
            int64_t ts = (int64_t)(target * AV_TIME_BASE);
            av_seek_frame(st->fmt_ctx, -1, ts, AVSEEK_FLAG_BACKWARD);
            avcodec_flush_buffers(st->codec_ctx);
            if (st->audio_codec_ctx) avcodec_flush_buffers(st->audio_codec_ctx);
            ctx->queue.clear();       
            got_first_pts = false;
            first_pts = 0;
            ctx->loop_restart.store(true); 
            continue;
        }

        int read_ret = av_read_frame(st->fmt_ctx, pkt);

        if (read_ret < 0) {
            if (ctx->looped.load()) {
                av_seek_frame(st->fmt_ctx, st->video_stream_idx, 0, AVSEEK_FLAG_BACKWARD);
                avcodec_flush_buffers(st->codec_ctx);
                if (st->audio_codec_ctx) avcodec_flush_buffers(st->audio_codec_ctx);
                got_first_pts = false;
                ctx->loop_restart.store(true);
                continue;
            } else {
                ctx->decoding.store(false);
                break;
            }
        }

        if (pkt->stream_index == st->video_stream_idx) {
            if (avcodec_send_packet(st->codec_ctx, pkt) >= 0) {
                while (ctx->decoding.load()) {
                    int recv_ret = avcodec_receive_frame(st->codec_ctx, frame);
                    if (recv_ret == AVERROR(EAGAIN) || recv_ret == AVERROR_EOF) break;
                    if (recv_ret < 0) break;

                    if (!ctx->isVulkan) {
                        AVFrame* yuv = alloc_yuv420p_frame(width, height);
                        if (!yuv) break;
                        sws_scale(st->sws_ctx, frame->data, frame->linesize, 0, height,
                                yuv->data, yuv->linesize);

                        int64_t pts = frame->best_effort_timestamp;
                        if (pts == AV_NOPTS_VALUE) pts = 0;
                        if (!got_first_pts) { first_pts = pts; got_first_pts = true; }

                        QueuedFrame qf{yuv, (pts - first_pts) * time_base};
                        if (!ctx->queue.push(qf)) { av_frame_free(&yuv); break; }
                    } else {
                        AVFrame* rgba = alloc_rgba_frame(width, height);
                        if (!rgba) break;
                        sws_scale(st->sws_ctx, frame->data, frame->linesize, 0, height,
                                rgba->data, rgba->linesize);

                        int64_t pts = frame->best_effort_timestamp;
                        if (pts == AV_NOPTS_VALUE) pts = 0;
                        if (!got_first_pts) { first_pts = pts; got_first_pts = true; }

                        QueuedFrame qf{rgba, (pts - first_pts) * time_base};
                        if (!ctx->queue.push(qf)) { av_frame_free(&rgba); break; }
                    }
                }
            }
        } else if (pkt->stream_index == st->audio_stream_idx && st->audio_codec_ctx) {
            handle_audio_packet(st, pkt, ctx->audio_device, audio_frame, &resampled_buf, ctx->volume.load());
        }
        av_packet_unref(pkt);
    }

    if (resampled_buf) av_freep(&resampled_buf);
    av_frame_free(&audio_frame);
    av_frame_free(&frame);
    av_packet_free(&pkt);
}

Video& createVideo(Window* win, Rect rect, const char* path, const std::string& name) { 
    Video v{};
    v.name = name;
    v.ctx = std::make_unique<VideoDecoderContext>();

    v.ctx->isVulkan = win->isVulkan;
 
    if (!open_video(path, v.ctx->decoderState, win->isVulkan)) {
        v.ctx->decoding.store(false);
        mainSpace.videos.push_back(std::move(v));
        
        return mainSpace.videos.back();
    }

    open_audio(v.ctx->decoderState);

    if (v.ctx->decoderState.audio_stream_idx >= 0) {
        SDL_AudioSpec want{}, have{};
        want.freq = v.ctx->decoderState.out_sample_rate;
        want.format = AUDIO_S16SYS;
        want.channels = v.ctx->decoderState.out_channels;
        want.samples = 1024;
        v.ctx->audio_device = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
        
        if (v.ctx->audio_device) SDL_PauseAudioDevice(v.ctx->audio_device, 0);
     }
 
    v.width = v.ctx->decoderState.codec_ctx->width;
    v.height = v.ctx->decoderState.codec_ctx->height;
 
    if (!win->isVulkan) {
        SDL_Texture* tex = SDL_CreateTexture(
        win->renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
        v.width, v.height);
        
        if (!tex) {
            fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
            v.ctx->decoding.store(false);
            mainSpace.videos.push_back(std::move(v));
            
            return mainSpace.videos.back();
        }

        v.texture.reset(tex);
    }

    static std::atomic<uint64_t> nextVideoId{1};
    v.id = nextVideoId++;
    v.renderer = win->renderer;
    v.win = win;
    v.displayRect = { rect.x, rect.y, rect.w, rect.h };
    v.playback_start = av_gettime();
    v.decoderThread = std::thread(decoder_thread_func, v.ctx.get());
 
    mainSpace.videos.push_back(std::move(v));
    return mainSpace.videos.back();
}

void updateVideoFrame(Video& v, const SDL_Rect* dst_rect = nullptr) {
    if (!v.ctx->decoding) return; 
    
    if (!v.ctx || !v.texture || !v.renderer) {
        return;
    }

    double now_seconds = (av_gettime() - v.playback_start) / 1000000.0;
    QueuedFrame qf;
    bool got = v.ctx->queue.try_pop_ready(now_seconds, qf);

    if (got) {
        SDL_UpdateYUVTexture(v.texture.get(), nullptr,
            qf.frame->data[0], qf.frame->linesize[0],
            qf.frame->data[1], qf.frame->linesize[1],
            qf.frame->data[2], qf.frame->linesize[2]);
        av_frame_free(&qf.frame);
    }

    SDL_RenderCopy(v.renderer, v.texture.get(), nullptr, dst_rect);
}

void setBackground(Color clr, Window* win) {
    mainSpace.backgrounds.emplace_back(clr, win);

    if (win->isVulkan) win->eng.changeBackground(clr);
}

void addRoundCorners(std::string objName, int radius) {
    for (auto& el : mainSpace.uiElements) {
        if (std::holds_alternative<Box>(el) && std::get<Box>(el).name == objName) {
            std::get<Box>(el).rad = radius; return;
        }
        if (std::holds_alternative<ScrollBar>(el) && std::get<ScrollBar>(el).name == objName) {
            std::get<ScrollBar>(el).rad = radius; return;
        }
    }

    SDL_Log("Object doesn't exist");
}

void DeleteObject(const std::string& name) {
    auto it = std::find_if(mainSpace.uiElements.begin(), mainSpace.uiElements.end(),
        [&](UIElement& el){ return std::visit(GetName{}, el) == name; });
    if (it == mainSpace.uiElements.end()) { SDL_Log("DeleteObject: '%s' not found", name.c_str()); return; }

    std::visit([&](auto& e){
        using T = std::decay_t<decltype(e)>;

        if constexpr(std::is_same_v<T,Text>) {
            if (e.win && e.win->isVulkan) {
                e.win->eng.releaseFontAtlas(e.pathToFont, e.fontSize);
            } else {
                if (e.font) TTF_CloseFont(e.font);
                if (e.tex)  SDL_DestroyTexture(e.tex);
                if (e.surf) SDL_FreeSurface(e.surf);
            }
        }
        if constexpr(std::is_same_v<T,Image>) {
            if (e.win && e.win->isVulkan) {
                e.win->eng.releaseImage(e.pathToImage);  
            } else {
                if (e.tex) SDL_DestroyTexture(e.tex);
            }
        }
        if constexpr(std::is_same_v<T,InputBox>) {
            if (e.win && e.win->isVulkan) {
                e.win->eng.releaseFontAtlas(e.pathToFont, e.fontSize);
            } else {
                if (e.font) TTF_CloseFont(e.font);
            }
        }
        if constexpr(std::is_same_v<T,Box>) {
            if (!(e.win && e.win->isVulkan)) {
                if (e.aaCache) SDL_DestroyTexture(e.aaCache);
            }
        }
    }, *it);

    mainSpace.uiElements.erase(it);
    auto ni = std::find(mainSpace.names.begin(), mainSpace.names.end(), name);
    if (ni != mainSpace.names.end()) mainSpace.names.erase(ni);
}

std::string getObjectProperty(std::string propName, std::string objectName) {
    auto fmtR4 = [](uint16_t x,uint16_t y,uint16_t w,uint16_t h){
        return std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(w)+", "+std::to_string(h); };
    auto fmtR3 = [](int x,int y,int s){
        return std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(s); };
    auto fmtC  = [](uint8_t r,uint8_t g,uint8_t b,uint8_t a){
        return std::to_string(r)+", "+std::to_string(g)+", "+std::to_string(b)+", "+std::to_string(a); };

    for (auto& el : mainSpace.uiElements) {
        if (std::visit(GetName{}, el) != objectName) continue;

        return std::visit([&](auto& e) -> std::string {
            using T = std::decay_t<decltype(e)>;

            auto winName = [&]() -> std::string {
                if constexpr(requires{ e.win; })
                    return e.win ? e.win->name : "";
                return "";
            };

            if (propName=="Window"||propName=="window") return winName();

            if constexpr(std::is_same_v<T,Box>) {
                if (propName=="Rect"||propName=="rect")   return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Color"||propName=="color") return fmtC(e.color.r,e.color.g,e.color.b,e.color.a);
            }
            else if constexpr(std::is_same_v<T,Circle>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect")
                    return fmtR3(e.uRect.x,e.uRect.y,e.uRect.size);
                if (propName=="Color"||propName=="color") return fmtC(e.clr.r,e.clr.g,e.clr.b,e.clr.a);
            }
            else if constexpr(std::is_same_v<T,Text>) {
                if (propName=="Rect"||propName=="rect")   return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Color"||propName=="color") return fmtC(e.color.r,e.color.g,e.color.b,e.color.a);
                if (propName=="FontSize"||propName=="fontSize"||propName=="Size"||propName=="size")
                    return std::to_string(e.fontSize);
                if (propName=="Font"||propName=="font")   return e.pathToFont;
                if (propName=="Text"||propName=="text")   return e.text;
            }
            else if constexpr(std::is_same_v<T,Image>) {
                if (propName=="Rect"||propName=="rect")   return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Image"||propName=="image"||propName=="path"||propName=="Path")
                    return e.pathToImage;
            }
            else if constexpr(std::is_same_v<T,InputBox>) {
                if (propName=="Rect"||propName=="rect")   return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Color"||propName=="color") return fmtC(e.clr.r,e.clr.g,e.clr.b,e.clr.a);
                if (propName=="Text"||propName=="text")   return e.buf;
                if (propName=="Placeholder"||propName=="placeholder") return e.hint;
                if (propName=="Caret"||propName=="caret") return std::to_string(e.caret);
                if (propName=="Focus"||propName=="focus") return e.focus ? "true" : "false";
                if (propName=="FontSize"||propName=="fontSize"||propName=="Size"||propName=="size")
                    return std::to_string(e.fontSize);
                if (propName=="Font"||propName=="font")   return e.pathToFont;
            }
            else if constexpr(std::is_same_v<T,CheckBox>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect")
                    return fmtR3(e.uRect.x,e.uRect.y,e.uRect.size);
                if (propName=="isOn") return e.isOn ? "true" : "false";
                if (propName=="Color"||propName=="color") return fmtC(e.clr.r,e.clr.g,e.clr.b,e.clr.a);
            }
            else if constexpr(std::is_same_v<T,RadioButton>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect")
                    return fmtR3(e.uRect.x,e.uRect.y,e.uRect.size);
                if (propName=="isOn") return e.status ? "true" : "false";
                if (propName=="Group"||propName=="group") return std::to_string(e.group);
                if (propName=="ColorOff"||propName=="colorOff") return fmtC(e.clrOff.r,e.clrOff.g,e.clrOff.b,e.clrOff.a);
                if (propName=="ColorOn"||propName=="colorOn")   return fmtC(e.clrOn.r,e.clrOn.g,e.clrOn.b,e.clrOn.a);
            }
            else if constexpr(std::is_same_v<T,Line>) {
                if (propName=="Rect"||propName=="rect") return fmtR4(e.cords.x,e.cords.y,e.cords.w,e.cords.h);
                if (propName=="Color"||propName=="color") return fmtC(e.color.r,e.color.g,e.color.b,e.color.a);
            }
            else if constexpr(std::is_same_v<T,Slider>) {
                if (propName=="Rect"||propName=="rect") return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Progress"||propName=="progress") return std::to_string(e.progress);
                if (propName=="ColorOn"||propName=="colorOn")   return fmtC(e.clrOn.r,e.clrOn.g,e.clrOn.b,e.clrOn.a);
                if (propName=="ColorOff"||propName=="colorOff") return fmtC(e.clrOff.r,e.clrOff.g,e.clrOff.b,e.clrOff.a);
            }
            else if constexpr(std::is_same_v<T,RangeSlider>) {
                if (propName=="Rect"||propName=="rect") return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Progress"||propName=="progress")   return std::to_string(e.progress);
                if (propName=="Progress2"||propName=="progress2") return std::to_string(e.progress2);
                if (propName=="ColorOn"||propName=="colorOn")   return fmtC(e.clrOn.r,e.clrOn.g,e.clrOn.b,e.clrOn.a);
                if (propName=="ColorOff"||propName=="colorOff") return fmtC(e.clrOff.r,e.clrOff.g,e.clrOff.b,e.clrOff.a);
            }
            else if constexpr(std::is_same_v<T,ScrollBar>) {
                if (propName=="Rect"||propName=="rect") return fmtR4(e.rect.x,e.rect.y,e.rect.w,e.rect.h);
                if (propName=="Color"||propName=="color") return fmtC(e.color.r,e.color.g,e.color.b,e.color.a);
                if (propName=="ScrollingSize"||propName=="scrollingSize") return std::to_string(e.scrollingSize);
                if (propName=="Pos"||propName=="pos"||propName=="Position"||propName=="position")
                    return std::to_string(e.pos);
            }
            return "";
        }, el);
    }

    SDL_Log("getObjectProperty: object or property not found.");

    return "";
}

void changeObjectProperties(std::string propName, std::string newVal, std::string objectName) {
    auto parseC = [&](Color& c) {
        int16_t r,g,b,a;
        if (parseNums4(newVal,r,g,b,a)) c={uint8_t(r),uint8_t(g),uint8_t(b),uint8_t(a)};
        else std::cerr<<"ERROR: Wrong property value.\n";
    };

    auto parseR4 = [&](Rect& r) {
        int16_t x,y,w,h;
        if (parseNums4(newVal,x,y,w,h)) r={x,y,w,h};
        else std::cerr<<"ERROR: Wrong property value.\n";
    };

    auto parseR3 = [&](Square& r) {
        int16_t x,y,s;
        if (parseNums3(newVal,x,y,s)) r={(int)x,(int)y,(int)s};
        else std::cerr<<"ERROR: Wrong property value.\n";
    };

    auto reloadFont = [&](TTF_Font*& font, const std::string& path, int size) {
        if (font) TTF_CloseFont(font);
        font = TTF_OpenFont(resolveFontPath(path).c_str(), size);
    };

    for (auto& win : mainSpace.wins) {
        if (win->name != objectName) continue;
        if (propName=="Name"||propName=="name") {
            for (auto& n : mainSpace.names) if (n==win->name) { n=newVal; break; }
            win->name = newVal;
        }
        return;
    }

    for (auto& el : mainSpace.uiElements) {
        if (std::visit(GetName{}, el) != objectName) continue;

        std::visit([&](auto& e){
            using T = std::decay_t<decltype(e)>;

            if (propName=="Name"||propName=="name") {
                for (auto& n : mainSpace.names) if (n==e.name) { n=newVal; break; }
                e.name = newVal;
                return;
            }

            if (propName=="Window"||propName=="window") {
                if constexpr (std::is_same_v<T,RadioButton>) {
                    for (auto& win : mainSpace.wins)
                        if (win->name==newVal) { e.win=win; return; }
                }
                else if constexpr (std::is_same_v<T,Box> || std::is_same_v<T,Circle> ||
                                    std::is_same_v<T,Text>   || std::is_same_v<T,Image> ||
                                    std::is_same_v<T,InputBox> || std::is_same_v<T,CheckBox>) {
                    for (auto& win : mainSpace.wins)
                        if (win->name==newVal) { e.win=win; return; }
                }
                return;
            }

            if constexpr(std::is_same_v<T,Box>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.rect); e.aaDirty = true; return; }
                if (propName=="Color"||propName=="color") { parseC(e.color); e.aaDirty = true; return; }
            }
            else if constexpr(std::is_same_v<T,Circle>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect") { parseR3(e.uRect); return; }
                if (propName=="Color"||propName=="color") { parseC(e.clr); return; }
            }
            else if constexpr(std::is_same_v<T,Text>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.rect); return; }
                if (propName=="Color"||propName=="color") { parseC(e.color); return; }
                if (propName=="Text"||propName=="text") {
                    e.text = newVal;

                    if (!e.win->isVulkan) {
                        if (e.surf) SDL_FreeSurface(e.surf);
                        if (e.tex)  SDL_DestroyTexture(e.tex);
                        e.surf = TTF_RenderUTF8_Blended_Wrapped(e.font, e.text.c_str(),
                            {e.color.r,e.color.g,e.color.b,e.color.a}, 0);
                        e.tex  = e.surf ? SDL_CreateTextureFromSurface(e.win->renderer, e.surf) : nullptr;
                    }

                    return;
                }
                if (propName=="Font"||propName=="font") {
                    e.pathToFont = newVal;

                    if (!e.win->isVulkan) {
                        reloadFont(e.font, e.pathToFont, e.fontSize);
                    }

                    return;
                }
                if (propName=="FontSize"||propName=="fontSize"||propName=="Size"||propName=="size") {
                    e.fontSize = std::stoi(newVal);

                    if (!e.win->isVulkan) {
                        reloadFont(e.font, e.pathToFont, e.fontSize);
                    }

                    return;
                }
            }
            else if constexpr(std::is_same_v<T,Image>) {
                if (propName=="Rect"||propName=="rect") { parseR4(e.rect); return; }
                if (propName=="Image"||propName=="image"||propName=="path"||propName=="Path") {
                    e.pathToImage = newVal;

                    if (!e.win->isVulkan) {
                        if (e.tex) SDL_DestroyTexture(e.tex);
                        e.tex = IMG_LoadTexture(e.win->renderer, e.pathToImage.c_str());
                    }

                    return;
                }
            }
            else if constexpr(std::is_same_v<T,InputBox>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.rect); return; }
                if (propName=="Color"||propName=="color") { parseC(e.clr);   return; }
                if (propName=="Text"||propName=="text")   {
                    e.buf = newVal;
                    return;
                }

                if (propName=="Placeholder"||propName=="placeholder") { e.hint = newVal; return; }
                if (propName=="Caret"||propName=="caret") { e.caret = std::stoi(newVal); return; }
                if (propName=="Font"||propName=="font") {
                    e.pathToFont = newVal;

                    if (!e.win->isVulkan) {
                        reloadFont(e.font, e.pathToFont, e.fontSize);
                    }

                    return;
                }
                if (propName=="FontSize"||propName=="fontSize"||propName=="Size"||propName=="size") {
                    e.fontSize = std::stoi(newVal);

                    if (!e.win->isVulkan) {
                        reloadFont(e.font, e.pathToFont, e.fontSize);
                    }

                    return;
                }
            }
            else if constexpr(std::is_same_v<T,CheckBox>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect") { parseR3(e.uRect); return; }
                if (propName=="Color"||propName=="color") { parseC(e.clr); return; }
                if (propName=="isOn") { e.isOn = (newVal=="true"||newVal=="1"); return; }
            }
            else if constexpr(std::is_same_v<T,RadioButton>) {
                if (propName=="Rect"||propName=="rect"||propName=="uRect") { parseR3(e.uRect); return; }
                if (propName=="isOn") { e.status = (newVal=="true"||newVal=="1"); return; }
                if (propName=="Group"||propName=="group") { e.group = std::stoi(newVal); return; }
                if (propName=="ColorOff"||propName=="colorOff") { parseC(e.clrOff); return; }
                if (propName=="ColorOn"||propName=="colorOn")   { parseC(e.clrOn);  return; }
            }
            else if constexpr(std::is_same_v<T,Line>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.cords); return; }
                if (propName=="Color"||propName=="color") { parseC(e.color);  return; }
            }
            else if constexpr(std::is_same_v<T,Slider>) {
                if (propName=="Rect"||propName=="rect") { parseR4(e.rect); return; }
                if (propName=="Progress"||propName=="progress") {
                    float v = std::stof(newVal);
                    if (v>=0.f&&v<=1.f) e.progress=v; else std::cerr<<"Wrong Progress.\n";
                    return;
                }
                if (propName=="ColorOff"||propName=="colorOff") { parseC(e.clrOff); return; }
                if (propName=="ColorOn"||propName=="colorOn")   { parseC(e.clrOn);  return; }
            }
            else if constexpr(std::is_same_v<T,RangeSlider>) {
                if (propName=="Rect"||propName=="rect") { parseR4(e.rect); return; }
                if (propName=="Progress"||propName=="progress") {
                    float v = std::stof(newVal);
                    if (v>=0.f&&v<=1.f) e.progress=v; else std::cerr<<"Wrong Progress.\n";
                    return;
                }
                if (propName=="Progress2"||propName=="progress2") {
                    float v = std::stof(newVal);
                    if (v>=0.f&&v<=1.f) e.progress2=v; else std::cerr<<"Wrong Progress2.\n";
                    return;
                }
                if (propName=="ColorOff"||propName=="colorOff") { parseC(e.clrOff); return; }
                if (propName=="ColorOn"||propName=="colorOn")   { parseC(e.clrOn);  return; }
            }
            else if constexpr(std::is_same_v<T,ScrollBar>) {
                if (propName=="Rect"||propName=="rect")   { parseR4(e.rect);  return; }
                if (propName=="Color"||propName=="color") { parseC(e.color);  return; }
                if (propName=="ScrollingSize"||propName=="scrollingSize") {
                    float v = std::stof(newVal);
                    if (v>0.f&&v<1.f) e.scrollingSize=v; else std::cerr<<"Wrong ScrollingSize.\n";
                    return;
                }
                if (propName=="Pos"||propName=="pos"||propName=="Position"||propName=="position") {
                    float v = std::stof(newVal);
                    if (v>=0.f&&v<=1.f) e.pos=v; else std::cerr<<"Wrong Pos.\n";
                    return;
                }
            }
        }, el);
        return;
    }

    SDL_Log("changeObjectProperties: object '%s' not found.", objectName.c_str());
}

void playSound              (Sound& s)                    { ma_sound_start(&s.sound); }
void stopSound              (Sound& s)                    { ma_sound_stop (&s.sound); }
void setSoundLooping        (Sound& s)                    { ma_sound_set_looping(&s.sound, MA_TRUE); }
void setSoundUnlooped       (Sound& s)                    { ma_sound_set_looping(&s.sound, MA_FALSE); }
void setSoundVolume         (Sound& s, float v)           { ma_sound_set_volume(&s.sound, v); }
void setSoundPosition       (Sound& s, float x, float y)  { ma_sound_set_position(&s.sound, x, y, 0); }

void playVideo              (Video& v)                    { v.ctx->decoding = true; }
void stopSound              (Video& v)                    { v.ctx->decoding = false; }
void setVideoLooped         (Video& v)                    { v.ctx->looped   = true; }
void setVideoUnlooped       (Video& v)                    { v.ctx->looped   = false; }
void setVideoVolume         (Video& v, float vol)         { v.setVolume(vol); }
void setVideoPosition       (Video& v, float pos)         { v.seek(pos); }

AVFrame* alloc_rgba_frame2(int width, int height) {
    AVFrame* frame = av_frame_alloc();
    if (!frame)
        return nullptr;

    frame->format = AV_PIX_FMT_RGBA;
    frame->width  = width;
    frame->height = height;

    if (av_frame_get_buffer(frame, 32) < 0) {
        av_frame_free(&frame);
        return nullptr;
    }

    return frame;
}

Image& uploadFrameFromVideo(Window* win, const char* path, Rect rect, double seekSeconds = 1.0, std::string name = "") { 
    DecoderState st{};
    
    if (!open_video(path, st, win->isVulkan)) {
        st.cleanup();
        return std::get<Image>(mainSpace.uiElements.back());
    }

    int64_t ts = (int64_t)(seekSeconds * AV_TIME_BASE);
    av_seek_frame(st.fmt_ctx, -1, ts, AVSEEK_FLAG_BACKWARD);
    avcodec_flush_buffers(st.codec_ctx);

    AVPacket* pkt = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    AVFrame* result = nullptr;

    while (av_read_frame(st.fmt_ctx, pkt) >= 0) {
        if (pkt->stream_index == st.video_stream_idx) {
            if (avcodec_send_packet(st.codec_ctx, pkt) >= 0) {
                if (avcodec_receive_frame(st.codec_ctx, frame) == 0) {
                    result = alloc_rgba_frame2(st.codec_ctx->width, st.codec_ctx->height);
                    
                    if (result) {
                        sws_scale(st.sws_ctx, frame->data, frame->linesize, 0,
                                  st.codec_ctx->height, result->data, result->linesize);
                    }
                    
                    av_packet_unref(pkt);
                    break;
                }
            }
        }
        av_packet_unref(pkt);
    }

    mainSpace.uiElements.emplace_back(Image(rect,path,nullptr,name,win,result));
    mainSpace.names.push_back(name);

    av_frame_free(&frame);
    av_packet_free(&pkt);
    st.cleanup();

    return std::get<Image>(mainSpace.uiElements.back());
}

int getDisplayWidth (int idx)                         { SDL_DisplayMode dm{}; SDL_GetDesktopDisplayMode(idx,&dm); return dm.w; }
int getDisplayHeight(int idx)                         { SDL_DisplayMode dm{}; SDL_GetDesktopDisplayMode(idx,&dm); return dm.h; }

std::pair<int,int> getDisplaySize(int idx) {
    SDL_DisplayMode dm{}; SDL_GetDesktopDisplayMode(idx,&dm);
    return {dm.w, dm.h};
}

Surface*       getSurfaceFromWin(Window* win, int width, int height, int depth, Color mask)                                   { 
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, depth, mask.r, mask.g, mask.b, mask.a);

    if (surface == NULL) {
        SDL_Log("INFO: Failed to create surface: %s", SDL_GetError());
    }

    Surface* surf = (Surface*)surface;

    mainSpace.surfaces.push_back(surf);

    return surf;
}

void setPixel(Window* win, const char* name, int x, int y, Color color) {
    if (win->isVulkan) {
        win->eng.setPixelV(name, x, y, color);
        return;
    }
    for (auto& c : win->sdlCanvases) {
        if (c.name != name) continue;
        if (x < 0 || y < 0 || x >= c.w || y >= c.h) return;
        size_t off = (size_t)(y * c.w + x) * 4;
        c.pixels[off+0] = color.r;
        c.pixels[off+1] = color.g;
        c.pixels[off+2] = color.b;
        c.pixels[off+3] = color.a;
        c.dirty = true;
        return;
    }
}

Color getPixel(Window* win, const char* canvasName, int x, int y) {
    if (win->isVulkan) return win->eng.getPixelV(canvasName, x, y);
    for (auto& c : win->sdlCanvases) {
        if (c.name != canvasName) continue;
        if (x < 0 || y < 0 || x >= c.w || y >= c.h) return {};
        size_t off = (size_t)(y * c.w + x) * 4;
        return { c.pixels[off], c.pixels[off+1], c.pixels[off+2], c.pixels[off+3] };
    }
    return {};
}

void createCanvas(Window* win, const char* name, int w, int h) {
    mainSpace.canvases.push_back({w, h, name, win});

    if (win->isVulkan) {
        win->eng.createEditableImageV(name, w, h);
    } else {
        Window::SDLCanvas c;
        c.name = name; c.w = w; c.h = h;
        c.pixels.assign((size_t)w * h * 4, 0);
        c.tex = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA32,
                                   SDL_TEXTUREACCESS_STREAMING, w, h);
        SDL_SetTextureBlendMode(c.tex, SDL_BLENDMODE_BLEND);
        win->sdlCanvases.push_back(std::move(c));
    }
}

void renderCanvas(Window* win, const char* name, Rect rect) {
    if (win->isVulkan) {
        win->eng.drawImageV(name, (float)rect.x, (float)rect.y, rect.w, rect.h);
        return;
    }

    for (auto& c : win->sdlCanvases) {
        if (c.name != name) continue;
        if (c.dirty) {
            SDL_UpdateTexture(c.tex, nullptr, c.pixels.data(), c.w * 4);
            c.dirty = false;
        }
        SDL_Rect dst = {rect.x, rect.y, rect.w, rect.h};
        SDL_RenderCopy(win->renderer, c.tex, nullptr, &dst);
        return;
    }
}

void flush(Window* win) {
    if (win->isVulkan) {
        win->eng.flush();
    } else {
        SDL_RenderClear(win->renderer);
    }
}

void drawFrame(Window* win) {
    if (win->isVulkan) {
        win->eng.drawFrame();
    } else {
        SDL_RenderPresent(win->renderer);
    }
}

const uint8_t* GetKeyboardState(int* n)                                         { return SDL_GetKeyboardState(n); }
Keymod         GetModState()                                                    { return (Keymod)SDL_GetModState(); }
void           SetModState(Keymod modstate)                                     { return SDL_SetModState((SDL_Keymod)modstate); }
void           ResetKeyboard()                                                  { return SDL_ResetKeyboard(); }
Keycode        GetKeyFromScancode(Scancode scancode)                            { return (Keycode)SDL_GetKeyFromScancode((SDL_Scancode)scancode); }
Scancode       GetScancodeFromKey(Keycode key)                                  { return (Scancode)SDL_GetScancodeFromKey((SDL_Keycode)key); }
const char*    GetKeyName(Keycode key)                                          { return SDL_GetKeyName((SDL_Keycode)key); }
Keycode        GetKeyFromName(const char* name)                                 { return (Keycode)SDL_GetKeyFromName(name); }
const char*    GetScancodeName(Scancode scancode)                               { return SDL_GetScancodeName((SDL_Scancode)scancode); }
Scancode       GetScancodeFromName(const char* name)                            { return (Scancode)SDL_GetScancodeFromName(name); }
bool           HasScreenKeyboardSupport()                                       { return SDL_HasScreenKeyboardSupport(); }
bool           IsScreenKeyboardShown(Window* window)                            { return SDL_IsScreenKeyboardShown(window->win); }
bool           IsTextInputActive()                                              { return SDL_IsTextInputActive(); }
void           StartTextInput()                                                 { return SDL_StartTextInput(); }
void           StopTextInput()                                                  { return SDL_StopTextInput(); }
void           SetTextInputRect(Rect* rect)                                     { return SDL_SetTextInputRect((SDL_Rect*)rect); }
void           ClearComposition()                                               { return SDL_ClearComposition(); }
bool           IsTextInputShown()                                               { return SDL_IsTextInputShown(); }

Window*        GetMouseFocus()                                                  { return (Window*)SDL_GetMouseFocus(); }
uint32_t       GetMouseState(int* x, int* y)                                    { return SDL_GetMouseState(x, y); }
uint32_t       GetGlobalMouseState(int* x, int* y)                              { return SDL_GetGlobalMouseState(x, y); }
uint32_t       GetRelativeMouseState(int* x, int* y)                            { return SDL_GetRelativeMouseState(x, y); }
void           WarpMouseInWindow(Window* window, int x, int y)                  { return SDL_WarpMouseInWindow(window->win, x, y); }
int            WarpMouseGlobal(int x, int y)                                    { return SDL_WarpMouseGlobal(x, y); }
int            SetRelativeMouseMode(bool enabled)                               { return SDL_SetRelativeMouseMode((SDL_bool)enabled); }
bool           GetRelativeMouseMode()                                           { return SDL_GetRelativeMouseMode(); }
int            CaptureMouse(bool enabled)                                       { return SDL_CaptureMouse((SDL_bool)enabled); }

Cursor*        CreateCursor(const uint8_t* data, const uint8_t* mask,
                            int w, int h, int hot_x, int hot_y)                 { return (Cursor*)SDL_CreateCursor(data, mask, w, h, hot_x, hot_y); }
Cursor*        CreateColorCursor(Surface* surface, int hot_x, int hot_y)        { return (Cursor*)SDL_CreateColorCursor((SDL_Surface*)surface, hot_x, hot_y); }
Cursor*        CreateSystemCursor(SystemCursor id)                              { return (Cursor*)SDL_CreateSystemCursor((SDL_SystemCursor)id); }
void           SetCursor(Cursor* cursor)                                        { return SDL_SetCursor((SDL_Cursor*)cursor); }
Cursor*        GetActiveCursor()                                                { return (Cursor*)SDL_GetCursor(); }
Cursor*        GetDefaultCursor()                                               { return (Cursor*)SDL_GetDefaultCursor(); }
void           FreeCursor(Cursor* cursor)                                       { return SDL_FreeCursor((SDL_Cursor*)cursor); }
int            SetCursorVisible(int toggle)                                     { return SDL_ShowCursor(toggle); }

uint64_t       GetPerformanceCounter()                                          { return SDL_GetPerformanceCounter(); }
uint64_t       GetPerformanceFrequency()                                        { return SDL_GetPerformanceFrequency(); }
uint32_t       GetTicks()                                                       { return SDL_GetTicks(); }
uint64_t       GetTicks64()                                                     { return SDL_GetTicks64(); }
void           Delay(uint32_t ms)                                               { return SDL_Delay(ms); }
TimerID        AddTimer(uint32_t interval, TimerCallback callback, void* param) { return (TimerID)SDL_AddTimer(interval, (SDL_TimerCallback)callback, param); }
bool           RemoveTimer(TimerID id)                                          { return SDL_RemoveTimer((SDL_TimerID)id); }

void entry() {}
void loop()  {}

int main(int argc, char** argv) {
    #ifndef _WIN32
        setenv("SDL_VIDEODRIVER", "wayland", 1);
    #endif
        signal(SIGINT, [](int){ mainSpace.running = false; });
    
    int lastW = 0, lastH = 0;

    auto init = [&]{
        if (SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
            SDL_Log("Init error: %s", SDL_GetError()); mainSpace.running=false;
        }
        if (TTF_Init() != 0) {
            SDL_Log("TTF error: %s", TTF_GetError()); mainSpace.running=false;
        }
        if (IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) == -1) {
            SDL_Log("Image error: %s", IMG_GetError()); mainSpace.running=false;
        }

        mainSpace.uiElements.clear();
        mainSpace.backgrounds.clear();
        mainSpace.names.clear();
        
        ma_engine_config cfg = ma_engine_config_init();
        cfg.noDevice = MA_TRUE;
        cfg.channels = 2;
        cfg.sampleRate = 44100;
        ma_engine_init(&cfg, &mainSpace.engine);

        av_log_set_level(AV_LOG_ERROR);
    };

    auto warnings = [&]{
        for (int i = 0; i < (int)mainSpace.names.size(); i++) {
            if (mainSpace.names[i].empty()) {
                std::cerr<<"ERROR: Object names cannot be empty.\n";
                mainSpace.running=false;
            }
            if (i+1 < (int)mainSpace.names.size() && mainSpace.names[i]==mainSpace.names[i+1]) {
                std::cerr<<"ERROR: Object names cannot be repeated.\n";
                mainSpace.running=false;
            }
        }

        std::set<int> groups;

        for (auto& el : mainSpace.uiElements)
            if (std::holds_alternative<RadioButton>(el))
                groups.insert(std::get<RadioButton>(el).group);

        for (int grp : groups) {
            int total=0, on=0;

            for (auto& el : mainSpace.uiElements) {
                if (!std::holds_alternative<RadioButton>(el)) continue;
                auto& r = std::get<RadioButton>(el);
                if (r.group!=grp) continue;
                total++; if (r.status) on++;
            }

            if (on==0||on==total) {
                SDL_Log("RadioButton group %d: all on or all off — fixing.", grp);

                for (auto& el : mainSpace.uiElements) {
                    if (!std::holds_alternative<RadioButton>(el)) continue;
                    auto& r = std::get<RadioButton>(el);
                    if (r.group==grp) { r.status=true; break; }
                }

                for (auto& el : mainSpace.uiElements) {
                    if (!std::holds_alternative<RadioButton>(el)) continue;
                    auto& r = std::get<RadioButton>(el);
                    if (r.group==grp && r.status) continue;
                    r.status=false;
                }
            }
        }
    };

    auto quit = [&]{
        for (auto& s : mainSpace.sounds) ma_sound_uninit(&s.sound);

        ma_engine_uninit(&mainSpace.engine);

        for (auto& el : mainSpace.uiElements) {
            std::visit([](auto& e){
                using T = std::decay_t<decltype(e)>;
                if constexpr(std::is_same_v<T,Text>) {
                    if (e.font) TTF_CloseFont(e.font);
                    if (e.tex)  SDL_DestroyTexture(e.tex);
                    if (e.surf) SDL_FreeSurface(e.surf);
                }
                if constexpr(std::is_same_v<T,Image>) {
                    if (e.tex) SDL_DestroyTexture(e.tex);
                }
                if constexpr(std::is_same_v<T,InputBox>) {
                    if (e.font) TTF_CloseFont(e.font);
                }
            }, el);
        }

        mainSpace.running = false;

        for (auto* w : mainSpace.wins) {
            if (!w) continue;
            if (w->renderer) { SDL_DestroyRenderer(w->renderer); w->renderer=nullptr; }
            if (w->win)      { SDL_DestroyWindow(w->win);        w->win=nullptr; }
            delete w;
        }

        for (auto* surf : mainSpace.surfaces) {
            SDL_FreeSurface((SDL_Surface*)surf);
        }

        TTF_Quit(); IMG_Quit(); SDL_Quit();

        return 0;
    };

    auto eventLoop = [&]{
        SDL_Event event;
        int   draggedSlider  = -1;
        int   draggedProgress = 1;
        int   SliderType     = 0;   
        bool  isScrollBar    = false;
        Uint64 lastTime = SDL_GetPerformanceCounter();

        while (mainSpace.running) {
            Uint64 now = GetPerformanceCounter();
            mainSpace.deltaTime = (float)(now - lastTime) / SDL_GetPerformanceFrequency();
            lastTime = now;

            mainSpace.Event = 0;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    mainSpace.Event |= EVENT_QUIT; mainSpace.running=false;
                }

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    mainSpace.Event |= EVENT_MOUSE_BUTTON_DOWN;

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;

                        InputBox& box = std::get<InputBox>(el);
                        
                        if (event.button.x>=box.rect.x && event.button.x<=(box.rect.x+box.rect.w) &&
                            event.button.y>=box.rect.y && event.button.y<=(box.rect.y+box.rect.h)) {
                            box.focus = true;

                            if (!box.font||box.buf.empty()) { box.caret=0; continue; }

                            int fh=0; TTF_SizeText(box.font,"A",nullptr,&fh);
                            int clickLine = (event.button.y-box.rect.y)/fh;
                            std::vector<std::string> lines; std::string cur;
                            
                            for (char c : box.buf) {
                                if (c=='\n') { lines.push_back(cur); cur.clear(); } else cur+=c;
                            }
                            
                            lines.push_back(cur);
                            clickLine = std::clamp(clickLine,0,(int)lines.size()-1);
                            int lineStart=0;
                            
                            for (int l=0;l<clickLine;l++) lineStart+=(int)lines[l].size()+1;
                            
                            const std::string& line=lines[clickLine];
                            int bestPos=0,bestDist=INT_MAX;
                            
                            for (int j=0;j<=(int)line.size();j++) {
                                int w=0; TTF_SizeText(box.font,line.substr(0,j).c_str(),&w,nullptr);
                                int d=std::abs(event.button.x-(box.rect.x+w));
                                if (d<bestDist){bestDist=d;bestPos=j;}
                            }

                            SDL_Keymod mod=SDL_GetModState();

                            if (mod&KMOD_SHIFT) {
                                if (box.selectStart==-1) box.selectStart=box.caret;
                                box.selectEnd=lineStart+bestPos; box.caret=box.selectEnd;
                            } else {
                                box.selectStart=-1; box.selectEnd=-1;
                                box.caret=lineStart+bestPos;
                            }
                        } else { box.focus=false; }
                    }

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<CheckBox>(el)) continue;
                        
                        CheckBox& box=std::get<CheckBox>(el);
                        auto& u=box.uRect;
                        
                        if (event.button.x>=u.x&&event.button.x<=(u.x+u.size)&&
                            event.button.y>=u.y&&event.button.y<=(u.y+u.size)) {
                            box.isOn =! box.isOn; 
                            if (!box.isAnim) box.isAnim = true;
                        }
                    }

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<RadioButton>(el)) continue;
                        
                        RadioButton& r=std::get<RadioButton>(el);
                        int dx=event.button.x-r.uRect.x, dy=event.button.y-r.uRect.y;
                        
                        if (dx*dx+dy*dy <= r.uRect.size*r.uRect.size && !r.status) {
                            mainSpace.RatioOnNow = r.name;
                            r.status=true; r.animPending=true;
                            for (auto& el2 : mainSpace.uiElements) {
                                if (!std::holds_alternative<RadioButton>(el2)) continue;
                                RadioButton& r2=std::get<RadioButton>(el2);
                                if (r2.name!=r.name&&r2.group==r.group&&r2.status) {
                                    r2.status=false; r2.animPending=true;
                                }
                            }
                        }
                    }

                    isScrollBar=false; draggedSlider=-1;
                    
                    for (int i=0;i<(int)mainSpace.uiElements.size();i++) {
                        if (!std::holds_alternative<ScrollBar>(mainSpace.uiElements[i])) continue;
                        ScrollBar& sb=std::get<ScrollBar>(mainSpace.uiElements[i]);
                        int tx1=sb.rect.x+(int)(sb.rect.w*0.9f);
                        int ty1=sb.rect.y+(int)(sb.pos*(sb.rect.h-sb.scrollingSize*sb.rect.h));
                        int tx2=sb.rect.x+sb.rect.w;
                        int ty2=ty1+(int)(sb.scrollingSize*sb.rect.h);
                        if (event.button.x>=tx1&&event.button.x<=tx2&&
                            event.button.y>=ty1&&event.button.y<=ty2) {
                            draggedSlider=i; isScrollBar=true; break;
                        }
                    }

                    if (draggedSlider<0) {
                        for (int i=0;i<(int)mainSpace.uiElements.size();i++) {
                            if (!std::holds_alternative<Slider>(mainSpace.uiElements[i])) continue;
                            Slider& s=std::get<Slider>(mainSpace.uiElements[i]);
                            int dx=event.button.x-(s.rect.x+(int)(s.rect.w*s.progress));
                            int dy=event.button.y-(s.rect.y+s.rect.h/2);
                            if (dx*dx+dy*dy<=s.rect.h*s.rect.h) {
                                draggedSlider=i; SliderType=0; break;
                            }
                        }
                    }
                    
                    if (draggedSlider<0) {
                        for (int i=0;i<(int)mainSpace.uiElements.size();i++) {
                            if (!std::holds_alternative<RangeSlider>(mainSpace.uiElements[i])) continue;
                            RangeSlider& s=std::get<RangeSlider>(mainSpace.uiElements[i]);
                            int dy=event.button.y-(s.rect.y+s.rect.h/2);
                            int dx =event.button.x-(s.rect.x+(int)(s.rect.w*s.progress));
                            int dx2=event.button.x-(s.rect.x+(int)(s.rect.w*s.progress2));
                            if (dx*dx+dy*dy<=s.rect.h*s.rect.h) {
                                draggedSlider=i; draggedProgress=0; SliderType=1; break;
                            }
                            if (dx2*dx2+dy*dy<=s.rect.h*s.rect.h) {
                                draggedSlider=i; draggedProgress=1; SliderType=1; break;
                            }
                        }
                    }
                }

                else if (event.type == SDL_TEXTINPUT) {
                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;
                        InputBox& box = std::get<InputBox>(el);
                        if (!box.focus) continue;

                        if (box.selectStart != -1 && box.selectEnd != -1 && box.selectStart != box.selectEnd) {
                            int selMin = std::min(box.selectStart, box.selectEnd);
                            int selMax = std::max(box.selectStart, box.selectEnd);
                            box.buf.erase(selMin, selMax - selMin);
                            box.caret = selMin;
                            box.selectStart = box.selectEnd = -1;
                        }

                        std::string ch = event.text.text; 
                        box.buf.insert(box.caret, ch);
                        box.caret += (int)ch.size();
                        box.changed = true;
                    }
                }

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    auto utf8CharLen = [&](const std::string& s, size_t pos) {
                        if (pos >= s.size()) return 1;
                        unsigned char c = s[pos];
                        if ((c & 0x80) == 0x00) return 1;     
                        if ((c & 0xE0) == 0xC0) return 2;      
                        if ((c & 0xF0) == 0xE0) return 3;    
                        if ((c & 0xF8) == 0xF0) return 4;     
                        return 1;
                    };

                    auto measureW = [&](InputBox& box, const std::string& s) -> int {
                        if (s.empty()) return 0;
                        if (box.win->isVulkan) {
                            return (int)box.win->eng.measureTextV(box.pathToFont.c_str(), s.c_str(), box.fontSize);
                        } else {
                            int w = 0, h = 0;
                            TTF_SizeUTF8(box.font, s.c_str(), &w, &h);
                            return w;
                        }
                    };

                    auto hitTestCaret = [&](InputBox& box, int mouseX) {
                        if (box.buf.empty()) return 0;

                        int relX = mouseX - box.rect.x;
                        if (relX <= 0) return 0;

                        int bestIdx = (int)box.buf.size();
                        int prevW = 0;
                        size_t prevI = 0;

                        for (size_t i = 0; i <= box.buf.size(); ) {
                            std::string sub = box.buf.substr(0, i);
                            int w = measureW(box, sub);

                            if (relX < w) {
                                bestIdx = (relX - prevW < w - relX) ? (int)prevI : (int)i;
                                break;
                            }
                            prevW = w;
                            prevI = i;
                            if (i == box.buf.size()) break;
                            i += utf8CharLen(box.buf, i);
                        }
                        return bestIdx;
                    };
                    
                    if (event.button.button != SDL_BUTTON_LEFT) break;

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;
                        InputBox& box = std::get<InputBox>(el);

                        SDL_Point p = {event.button.x, event.button.y};
                        SDL_Rect r = {box.rect.x, box.rect.y, box.rect.w, box.rect.h};

                        if (SDL_PointInRect(&p, &r)) {
                            box.focus = true;
                            SDL_StartTextInput();

                            int idx = hitTestCaret(box, event.button.x);
                            SDL_Keymod mod = SDL_GetModState();

                            if (mod & KMOD_SHIFT) {
                                if (box.selectStart == -1) box.selectStart = box.caret;
                                box.caret = idx;
                                box.selectEnd = idx;
                            } else {
                                box.caret = idx;
                                box.selectStart = box.selectEnd = -1;
                                box.dragging = true;
                                box.dragAnchor = idx;
                            }
                        } else {
                            box.focus = false;
                        }
                    }
                }

                else if (event.type == SDL_TEXTINPUT) {
                    if (SDL_GetModState() & KMOD_CTRL) continue;

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;
                        InputBox& box = std::get<InputBox>(el);
                        if (!box.focus) continue;

                        if (box.selectStart != -1 && box.selectEnd != -1 && box.selectStart != box.selectEnd) {
                            int selMin = std::min(box.selectStart, box.selectEnd);
                            int selMax = std::max(box.selectStart, box.selectEnd);
                            box.buf.erase(selMin, selMax - selMin);
                            box.caret = selMin;
                            box.selectStart = box.selectEnd = -1;
                        }

                        std::string ch = event.text.text; 
                        box.buf.insert(box.caret, ch);
                        box.caret += (int)ch.size();
                        box.changed = true;
                    }
                }

                if (event.type == SDL_KEYDOWN) {
                    mainSpace.Event |= EVENT_KEY_DOWN;

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;
                        InputBox& box = std::get<InputBox>(el);
                        if (!box.focus) continue;

                        SDL_Keymod mod = SDL_GetModState();
                        const char* name = SDL_GetKeyName(event.key.keysym.sym);
                        box.changed = true;

                        bool hasSel = (box.selectStart != -1 && box.selectEnd != -1 && box.selectStart != box.selectEnd);

                        if ((mod & KMOD_CTRL) && event.key.keysym.sym == SDLK_a) {
                            box.selectStart = 0;
                            box.selectEnd   = (int)box.buf.size();
                            box.caret       = (int)box.buf.size();
                            continue;
                        }

                        if ((mod & KMOD_CTRL) && event.key.keysym.sym == SDLK_c && hasSel) {
                            int selMin = std::min(box.selectStart, box.selectEnd);
                            int selMax = std::max(box.selectStart, box.selectEnd);
                            SDL_SetClipboardText(box.buf.substr(selMin, selMax - selMin).c_str());
                            continue;
                        }

                        if ((mod & KMOD_CTRL) && event.key.keysym.sym == SDLK_x && hasSel) {
                            int selMin = std::min(box.selectStart, box.selectEnd);
                            int selMax = std::max(box.selectStart, box.selectEnd);
                            SDL_SetClipboardText(box.buf.substr(selMin, selMax - selMin).c_str());
                            box.buf.erase(selMin, selMax - selMin);
                            box.caret = selMin;
                            box.selectStart = box.selectEnd = -1;
                            continue;
                        }

                        if ((mod & KMOD_CTRL) && event.key.keysym.sym == SDLK_v) {
                            if (hasSel) {
                                int selMin = std::min(box.selectStart, box.selectEnd);
                                int selMax = std::max(box.selectStart, box.selectEnd);
                                box.buf.erase(selMin, selMax - selMin);
                                box.caret = selMin;
                                box.selectStart = box.selectEnd = -1;
                            }
                            if (SDL_HasClipboardText()) {
                                char* clip = SDL_GetClipboardText();
                                if (clip && clip[0] != '\0') {
                                    std::string clipStr(clip);
                                    box.buf.insert(box.caret, clipStr);
                                    box.caret += (int)clipStr.size();
                                }
                                SDL_free(clip);
                            }
                            continue;
                        }

                        if (hasSel) {
                            bool isNav = (strcmp(name,"Left")==0 || strcmp(name,"Right")==0 ||
                                        strcmp(name,"Up")==0   || strcmp(name,"Down")==0  ||
                                        strcmp(name,"Left Shift")==0  || strcmp(name,"Right Shift")==0 ||
                                        strcmp(name,"Left Ctrl")==0   || strcmp(name,"Right Ctrl")==0);

                            if (!isNav) {
                                int selMin = std::min(box.selectStart, box.selectEnd);
                                int selMax = std::max(box.selectStart, box.selectEnd);
                                box.buf.erase(selMin, selMax - selMin);
                                box.caret = selMin;
                                box.selectStart = box.selectEnd = -1;
                                hasSel = false;
                                if (strcmp(name,"Backspace")==0 || strcmp(name,"Delete")==0) continue;
                            }
                        }

                        if (event.key.keysym.sym == SDLK_RETURN ||
                            event.key.keysym.sym == SDLK_RETURN2 ||
                            event.key.keysym.sym == SDLK_KP_ENTER) {
                            box.buf.insert(box.caret, "\n");
                            box.caret += 1;
                            continue;
                        }

                        if (event.key.keysym.sym == SDLK_BACKSPACE) {
                            if (box.caret > 0) {
                                int prev = box.caret - 1;
                                while (prev > 0 && (box.buf[prev] & 0xC0) == 0x80) prev--;
                                box.buf.erase(prev, box.caret - prev);
                                box.caret = prev;
                            }
                            continue;
                        }

                        if (event.key.keysym.sym == SDLK_DELETE) {
                            if (box.caret < (int)box.buf.size()) {
                                int next = box.caret + 1;
                                while (next < (int)box.buf.size() && (box.buf[next] & 0xC0) == 0x80) next++;
                                box.buf.erase(box.caret, next - box.caret);
                            }
                            continue;
                        }
                    }
                }

                if (event.type == SDL_MOUSEBUTTONUP) {
                    mainSpace.Event|=EVENT_MOUSE_BUTTON_UP; draggedSlider=-1;

                    if (event.button.button != SDL_BUTTON_LEFT) break;

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;
                        std::get<InputBox>(el).dragging = false;
                    }
                    break;
                }
                
                if (event.type==SDL_WINDOWEVENT) {
                    switch(event.window.event) {
                        case SDL_WINDOWEVENT_MAXIMIZED:       mainSpace.Event|=EVENT_WINDOWEVENT_MAXIMIZED; break;
                        case SDL_WINDOWEVENT_MINIMIZED:       mainSpace.Event|=EVENT_WINDOWEVENT_MINIMIZED; break;
                        case SDL_WINDOWEVENT_ENTER:           mainSpace.Event|=EVENT_WINDOWEVENT_ENTER; break;
                        case SDL_WINDOWEVENT_EXPOSED:         mainSpace.Event|=EVENT_WINDOWEVENT_EXPOSED; break;
                        case SDL_WINDOWEVENT_DISPLAY_CHANGED: mainSpace.Event|=EVENT_WINDOWEVENT_DISPLAY_CHANGED; break;
                        case SDL_WINDOWEVENT_FOCUS_LOST:      mainSpace.Event|=EVENT_WINDOWEVENT_FOCUS_LOST; break;
                        case SDL_WINDOWEVENT_LEAVE:           mainSpace.Event|=EVENT_WINDOWEVENT_LEAVE; break;
                        case SDL_WINDOWEVENT_SHOWN:           mainSpace.Event|=EVENT_WINDOWEVENT_SHOWN; break;
                        case SDL_WINDOWEVENT_TAKE_FOCUS:      mainSpace.Event|=EVENT_WINDOWEVENT_TAKE_FOCUS; break;
                        case SDL_WINDOWEVENT_SIZE_CHANGED:    mainSpace.Event|=EVENT_WINDOWEVENT_SIZE_CHANGED; break;
                        case SDL_WINDOWEVENT_RESIZED:         mainSpace.Event|=EVENT_WINDOW_RESIZED; break;
                    }
                }

                if (event.type==SDL_FINGERDOWN)          mainSpace.Event|=EVENT_FINGER_DOWN;
                if (event.type==SDL_KEYUP)               mainSpace.Event|=EVENT_KEY_UP;
                if (event.type==SDL_FINGERUP)            mainSpace.Event|=EVENT_FINGER_UP;
                if (event.type==SDL_FINGERMOTION)        mainSpace.Event|=EVENT_FINGER_MOTION;
                if (event.type==SDL_CLIPBOARDUPDATE)     mainSpace.Event|=EVENT_CLIPBOARD_UPDATE;
                if (event.type==SDL_DROPFILE)            mainSpace.Event|=EVENT_DROP_FILE;
                if (event.type==SDL_DROPTEXT)            mainSpace.Event|=EVENT_DROP_TEXT;
                if (event.type==SDL_DROPBEGIN)           mainSpace.Event|=EVENT_DROP_BEGIN;
                if (event.type==SDL_DROPCOMPLETE)        mainSpace.Event|=EVENT_DROP_COMPLETE;
                if (event.type==SDL_APP_TERMINATING)     mainSpace.Event|=EVENT_TERMINATING;
                if (event.type==SDL_APP_LOWMEMORY)       mainSpace.Event|=EVENT_LOW_MEMORY;
                if (event.type==SDL_APP_WILLENTERBACKGROUND) mainSpace.Event|=EVENT_WILL_ENTER_BACKGROUND;
                if (event.type==SDL_APP_WILLENTERFOREGROUND) mainSpace.Event|=EVENT_WILL_ENTER_FOREGROUND;
                if (event.type==SDL_APP_DIDENTERBACKGROUND)  mainSpace.Event|=EVENT_DID_ENTER_BACKGROUND;
                if (event.type==SDL_APP_DIDENTERFOREGROUND)  mainSpace.Event|=EVENT_DID_ENTER_FOREGROUND;
                if (event.type==SDL_JOYAXISMOTION)       mainSpace.Event|=EVENT_JOYSTICK_AXIS_MOTION;
                if (event.type==SDL_JOYBALLMOTION)       mainSpace.Event|=EVENT_JOYSTICK_BALL_MOTION;
                if (event.type==SDL_JOYBATTERYUPDATED)   mainSpace.Event|=EVENT_JOYSTICK_BATTERY_UPDATED;
                if (event.type==SDL_JOYBUTTONDOWN)       mainSpace.Event|=EVENT_JOYSTICK_BUTTON_DOWN;
                if (event.type==SDL_JOYBUTTONUP)         mainSpace.Event|=EVENT_JOYSTICK_BUTTON_UP;
                if (event.type==SDL_JOYDEVICEADDED)      mainSpace.Event|=EVENT_JOYSTICK_DEVICE_ADDED;
                if (event.type==SDL_JOYDEVICEREMOVED)    mainSpace.Event|=EVENT_JOYSTICK_DEVICE_REMOVED;
                if (event.type==SDL_JOYHATMOTION)        mainSpace.Event|=EVENT_JOYSTICK_HAT_MOTION;

                if (event.type==SDL_MOUSEMOTION) {
                    mainSpace.Event|=EVENT_MOUSE_MOTION;

                    auto utf8CharLen = [&](const std::string& s, size_t pos) {
                        if (pos >= s.size()) return 1;
                        unsigned char c = s[pos];
                        if ((c & 0x80) == 0x00) return 1;     
                        if ((c & 0xE0) == 0xC0) return 2;      
                        if ((c & 0xF0) == 0xE0) return 3;    
                        if ((c & 0xF8) == 0xF0) return 4;     
                        return 1;
                    };

                    auto measureW = [&](InputBox& box, const std::string& s) -> int {
                        if (s.empty()) return 0;
                        if (box.win->isVulkan) {
                            return (int)box.win->eng.measureTextV(box.pathToFont.c_str(), s.c_str(), box.fontSize);
                        } else {
                            int w = 0, h = 0;
                            TTF_SizeUTF8(box.font, s.c_str(), &w, &h);
                            return w;
                        }
                    };

                    auto hitTestCaret = [&](InputBox& box, int mouseX) {
                        if (box.buf.empty()) return 0;

                        int relX = mouseX - box.rect.x;
                        if (relX <= 0) return 0;

                        int bestIdx = (int)box.buf.size();
                        int prevW = 0;
                        size_t prevI = 0;

                        for (size_t i = 0; i <= box.buf.size(); ) {
                            std::string sub = box.buf.substr(0, i);
                            int w = measureW(box, sub);

                            if (relX < w) {
                                bestIdx = (relX - prevW < w - relX) ? (int)prevI : (int)i;
                                break;
                            }
                            prevW = w;
                            prevI = i;
                            if (i == box.buf.size()) break;
                            i += utf8CharLen(box.buf, i);
                        }
                        return bestIdx;
                    };
                    
                    bool leftDown = (event.motion.state & SDL_BUTTON_LMASK) != 0;

                    if (draggedSlider>=0) {
                        if (isScrollBar) {
                            ScrollBar& sb=std::get<ScrollBar>(mainSpace.uiElements[draggedSlider]);

                            if (sb.rect.h != (sb.rect.h / std::sqrt((float)std::max((size_t)1, sb.elements.size()))) * sb.scrollingSize) {
                                float slH=sb.rect.h / std::sqrt((float)std::max((size_t)1, sb.elements.size()));
                                float trH=sb.rect.h-slH;
                                sb.pos=std::clamp((float)(event.motion.y-sb.rect.y)/trH, 0.f,1.f);
                            }
                        } else if (SliderType==0) {
                            Slider& s=std::get<Slider>(mainSpace.uiElements[draggedSlider]);
                            s.progress=std::clamp((float)(event.motion.x-s.rect.x)/s.rect.w, 0.f,1.f);
                        } else {
                            RangeSlider& s=std::get<RangeSlider>(mainSpace.uiElements[draggedSlider]);
                            float np=std::clamp((float)(event.motion.x-s.rect.x)/s.rect.w, 0.f,1.f);
                            if (draggedProgress==0 && np<=s.progress2-0.1f) s.progress=np;
                            if (draggedProgress==1 && np>=s.progress +0.1f) s.progress2=np;
                        }
                    }

                    for (auto& el : mainSpace.uiElements) {
                        if (!std::holds_alternative<InputBox>(el)) continue;
                        InputBox& box = std::get<InputBox>(el);
                        if (!box.focus || !box.dragging) continue;

                         if (!leftDown) {    
                            box.dragging = false;
                            continue;
                        }

                        int idx = hitTestCaret(box, event.button.x);
                        if (idx != box.dragAnchor) {
                            box.selectStart = box.dragAnchor;
                            box.selectEnd = idx;
                            box.caret = idx;
                        } else {
                            box.selectStart = box.selectEnd = -1;
                        }
                    }
                }
            }

            for (auto& win : mainSpace.wins) {
                bool hasBg = false;
                
                for (auto& bg : mainSpace.backgrounds) {
                    if (std::get<1>(bg)->renderer==win->renderer) {
                        if (win->renderer != nullptr) SDL_SetRenderDrawColor(win->renderer,
                            std::get<0>(bg).r,std::get<0>(bg).g,std::get<0>(bg).b,std::get<0>(bg).a);
                        if (win->renderer != nullptr) hasBg = true; break;
                    }
                }

                if (!hasBg && win->renderer != nullptr) SDL_SetRenderDrawColor(win->renderer, 100,100,100,255);
                if (win->renderer != nullptr) SDL_RenderClear(win->renderer);
            }

            UpdateAll();

            for (auto& v : mainSpace.videos) {
                bool isVulkan = false;

                for (auto& win : mainSpace.wins) {
                    if (!win->isVulkan || win != v.win) continue;

                    isVulkan = true;
                    win->eng.drawVideoV(v, v.displayRect.x, v.displayRect.y, v.displayRect.w, v.displayRect.h);
                }

                if (!isVulkan) {  
                    updateVideoFrame(v, &v.displayRect); 
                }
            }

            loop();

            for (auto& win : mainSpace.wins) {
                if (win->eng.started && !win->eng.cleaned && !win->eng.presentedThisFrame) {
                    win->eng.flush();
                    win->eng.drawFrame();
                    bool presentedThisFrame = false;
                }

                if (win->renderer != nullptr) SDL_RenderPresent(win->renderer);
            }       
        }
    };

    init();
    entry();
    warnings();
    eventLoop();
    quit();

    return 0;
}
