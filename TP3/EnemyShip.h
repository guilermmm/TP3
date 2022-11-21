/**********************************************************************************
// Blue (Arquivo de Cabe�alho)
// 
// Cria��o:     10 Out 2012
// Atualiza��o: 01 Nov 2021
// Compilador:  Visual C++ 2019
//
// Descri��o:   Objeto faz uma persegui��o suavizada
//
**********************************************************************************/

#ifndef _GEOWARS_ENEMY_SHIP_H_
#define _GEOWARS_ENEMY_SHIP_H_

// ---------------------------------------------------------------------------------

#include "Object.h"    
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Player.h"
#include "Cooldown.h"

// ---------------------------------------------------------------------------------

class EnemyShip : public Object
{
private:
    Sprite * sprite;                    // sprite do objeto
    Vector * speed;                     // velocidade e dire��o
    Player * player;                    // ponteiro para jogador
    float factor;                       // fator de escala

    Cooldown attackCd{ 1.5f };
    
public:
    EnemyShip(Player * p);                   // construtor
    ~EnemyShip();                            // destrutor
    
    void OnCollision(Object* obj);      // resolu��o da colis�o
    void Update();                      // atualiza��o
    void Draw();                        // desenho
}; 

// ---------------------------------------------------------------------------------

inline void EnemyShip::Draw()
{ sprite->Draw(x, y, Layer::LOWER, scale, rotation); }

// ---------------------------------------------------------------------------------


#endif