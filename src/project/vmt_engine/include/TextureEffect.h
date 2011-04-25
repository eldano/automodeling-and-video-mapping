#ifndef TEXTUREEFFECT_H
#define TEXTUREEFFECT_H

#include "Effect.h"
#include "QuadGroup.h"
#include "Object3D.h"
//#include "LinearAnimation.h"

class TextureEffect : public Effect
{
    public:
        TextureEffect(QuadGroup *qg, string texturePath, textureType type);
        TextureEffect(Object3D *obj3d, string facesID, string texturePath, textureType type);
        virtual ~TextureEffect();

        virtual void play();
        virtual void test();
        virtual int getType();

        QuadGroup* getGroup();
        Object3D* getObject3D();
        string getFacesID();
        string getTextureKey();
        textureType getTextureType();
        bool getIsGroup();

    protected:
    private:
        QuadGroup* group;
        Object3D* object3d;
        string facesID;

        string textureKey;
        textureType type;

        bool isGroup;
};

#endif // TEXTUREEFFECT_H
