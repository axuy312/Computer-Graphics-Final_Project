float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f,  -1.0f, 
    -1.0f, -1.0f, -1.0f, 
    1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 
    1.0f,  1.0f,  -1.0f, 
    -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  
    -1.0f, -1.0f, -1.0f, 
    -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, 
    -1.0f, 1.0f,  1.0f,  
    -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 
    1.0f,  -1.0f, 1.0f,  
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  
    1.0f,  1.0f,  -1.0f, 
    1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  
    -1.0f, 1.0f,  1.0f,  
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  
    1.0f,  -1.0f, 1.0f,  
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 
    1.0f,  1.0f,  -1.0f, 
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  
    -1.0f, 1.0f,  1.0f,  
    -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, 
    -1.0f, -1.0f, 1.0f,  
    1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 
    -1.0f, -1.0f, 1.0f,  
    1.0f,  -1.0f, 1.0f
};

float blockVertices[] = {
    // front 
    1.0f, -1.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f,  1.0f,  -1.0f,
    1.0f,  -1.0f, -1.0f, 
    
    // left 
    -1.0f, 1.0f, -1.0f,  
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f,  1.0f,  
    -1.0f, 1.0f,  -1.0f,

    //right  
    1.0f, 1.0f, 1.0f,
    1.0f,  -1.0f, 1.0f,
    1.0f,  -1.0f, -1.0f, 
    1.0f,  -1.0f, -1.0f, 
    1.0f,  1.0f,  -1.0f, 
    1.0f,  1.0f,  1.0f, 

    //back
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,  1.0f,  
    -1.0f, -1.0f, 1.0f,  
    -1.0f, -1.0f, 1.0f,  
    1.0f,  -1.0f, 1.0f,  
    1.0f,  1.0f,  1.0f,

    //top
    1.0f, 1.0f, 1.0f,
    1.0f,  1.0f,  -1.0f, 
    -1.0f, 1.0f,  -1.0f, 
    -1.0f, 1.0f,  -1.0f,  
    -1.0f, 1.0f,  1.0f, 
    1.0f,  1.0f,  1.0f,

    //bot
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,  
    -1.0f, -1.0f, -1.0f, 
    1.0f,  -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f, 
    1.0f,  -1.0f, -1.0f
};

float blockNormals[] = {
    // front 
    0.0f, 0.0f, -1.0f, 
    0.0f, 0.0f, -1.0f, 
    0.0f, 0.0f, -1.0f, 
    0.0f, 0.0f, -1.0f, 
    0.0f, 0.0f, -1.0f, 
    0.0f, 0.0f, -1.0f,  
    
    // left 
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,  
    -1.0f, 0.0f, 0.0f,  

    //right  
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    //back
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    //top
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    //bot
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f
};

char blueSkyboxfaces[6][30] = {"../assets/skybox/right.png", "../assets/skybox/left.png",  "../assets/skybox/bot.png",
                               "../assets/skybox/top.png",   "../assets/skybox/front.png", "../assets/skybox/back.png"};