#include "HelloWorldScene.h"

USING_NS_CC;

namespace DrawOpengl
{
    static bool s_initialized = false;
    static GLProgram* s_shader = nullptr;
    static int s_colorLocation = -1;
    static Color4F s_color(1.0f,1.0f,1.0f,1.0f);
    static int s_pointSizeLocation = -1;
    
    static void lazy_init( void )
    {
        if( ! s_initialized ) {
            
            //
            // Position and 1 color passed as a uniform (to simulate glColor4ub )
            //
            s_shader = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
            s_shader->retain();
            
            s_colorLocation = glGetUniformLocation( s_shader->getProgram(), "u_color");
            CHECK_GL_ERROR_DEBUG();
            s_pointSizeLocation = glGetUniformLocation( s_shader->getProgram(), "u_pointSize");
            CHECK_GL_ERROR_DEBUG();
            
            s_initialized = true;
        }
    }
    void drawLine( const Point& origin, const Point& destination )
    {
        lazy_init();
        
        Vertex2F vertices[2] = {
            Vertex2F(origin.x, origin.y),
            Vertex2F(destination.x, destination.y)
        };
        
        s_shader->use();
        s_shader->setUniformsForBuiltins();
        s_shader->setUniformLocationWith4fv(s_colorLocation, (GLfloat*) &s_color.r, 1);
        
        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
#ifdef EMSCRIPTEN
        setGLBufferData(vertices, 16);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
#endif // EMSCRIPTEN
        glDrawArrays(GL_LINES, 0, 2);
        
        CC_INCREMENT_GL_DRAWS(1);
    }
}
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    return true;
}

void HelloWorld::draw()
{
    DrawOpengl::drawLine(Point(300,300),Point(600,300));
}


