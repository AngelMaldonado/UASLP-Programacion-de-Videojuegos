#ifndef TEXTURE_H
#define TEXTURE_H

#include "libraries.h"

class Texture 
{
public:
    int limite;
    unsigned int texture[1];

    Texture(unsigned int ourTexture[], int ourLimit)
    {
        limite = ourLimit;
        for (int i = 0; i < limite; i++)
        {
            texture[i] = ourTexture[i];
        }
    }

    void GeneraTextura(int n, string path, int td, int e)
    {
        glGenTextures(1, &texture[n]);
        glBindTexture(GL_TEXTURE_2D, texture[n]);

        if (e == 0)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else if (e == 1)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        }
        else if (e == 2)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else if (e == 3)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

            float borderColor[] = { 0.0f,0.0f,0.0f,0.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
        else
        {
            cout << "Valores de 0 a 3 en r" << endl;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            if (td == 0)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            cout << "No se puede cargar la textura" << endl;
        }
        stbi_image_free(data);
    }

    void ViewTexture()
    {
        for (int i = 0; i < limite; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texture[i]);
        }
    }

    string UniformTexture()
    {
        string completo = "";
        string str = "texture";
        string numero = "0";
        int n = stoi(numero);
        for (int i = 0; i < limite; i++)
        {           
            completo = "";
            n += 1;

            numero = to_string(n);
            
            completo.append(str);
            completo.append(numero);
        }
        return completo;
    }
};
#endif
