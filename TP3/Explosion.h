/**********************************************************************************
// WallHit (Arquivo de Cabe�alho)
//
// Cria��o:     03 Ago 2019
// Atualiza��o: 01 Nov 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   Efeito de explos�o de fa�scas usando sistema de part�culas
//
**********************************************************************************/

#ifndef _GEOWARS_EXPLOSION_H_
#define _GEOWARS_EXPLOSION_H_

// ---------------------------------------------------------------------------------

#include "Object.h"    
#include "Types.h"
#include "Particles.h"

// ---------------------------------------------------------------------------------

class Explosion : public Object
{
private:
    Particles* sparks;                 // sistema de part�culas

public:
    Explosion(float pX, float pY);        // construtor
    ~Explosion();                         // destrutor

    int  Size();                        // quantidade de part�culas
    void Update();                      // atualiza��o
    void Draw();                        // desenho
};

// ---------------------------------------------------------------------------------

inline int Explosion::Size()
{
    return sparks->Size();
}

inline void Explosion::Draw()
{
    sparks->Draw(Layer::MIDDLE, 0.0f);
}

// ---------------------------------------------------------------------------------


#endif