#include <Feng/Render/RenderBase.h>

#include <OpenGL/gl.h>

namespace Feng::Render
{
    void SetDrawFace(eDrawFace face)
    {
        glCullFace(GL_BACK);

        switch (face)
        {
            case eDrawFace::Cw:
                glFrontFace(GL_CW);
                glEnable(GL_CULL_FACE);
                break;

            case eDrawFace::Ccw:
                glFrontFace(GL_CCW);
                glEnable(GL_CULL_FACE);
                break;
                
            case eDrawFace::Both:
                glDisable(GL_CULL_FACE);
                break;
        }
    }
}
