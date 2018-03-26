//
// Created by matha on 26/03/2018.
//
#pragma once

#include <string>
#include <vector>
#include "vendor/imgui/imgui.h"
#include "vendor/tinydir/tinydir.h"
#include "Texture.h"
#include "Model.h"


class DirectoryInspector {
private:
    bool show;
    std::vector<Texture*> files;
    int selectedSubmesh = 0;

public:
    DirectoryInspector() {
        std::string texturesPath = "res/textures";

        tinydir_dir dir;
        tinydir_open(&dir, texturesPath.c_str());

        while(dir.has_next) {
            tinydir_file file;
            tinydir_readfile(&dir, &file);
            if(strstr(file.extension, "png") || strstr(file.extension, "jpg"))
            files.emplace_back(new Texture(file.path));

            tinydir_next(&dir);
        }

        tinydir_close(&dir);
    }

    void Draw(Model* model) {
        ImGui::Begin("Model Inspector", &show, ImGuiWindowFlags_NoNav);
        {
            for (int i = 0; i < model->SubmeshCount(); ++i) {
                if(ImGui::Button(model->GetSubmesh(i)->name.c_str())) {
                    selectedSubmesh = i;
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Textures", &show, ImGuiWindowFlags_NoNav);
        {
            for (int i = 0; i < files.size(); ++i) {
                if(ImGui::ImageButton((ImTextureID)files[i]->GetTextureId(), {100, 100})) {
                    model->GetMaterial(selectedSubmesh)->Diffuse = files[i];
                }
            }
        }
        ImGui::End();
    }
};