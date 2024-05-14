#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <assimp/Importer.hpp>
#include <memory>
#include <sys/sysinfo.h>
#include <sstream>
#include <fstream>
#include <unistd.h>

#include <Window.hpp>
#include <SceneNode.hpp>
#include <Texture.hpp>
#include <Plane.hpp>
#include <Camera.hpp>
#include <ModelCollider.hpp>
#include <Player.hpp>
#include <PointLight.hpp>
#include <DirectionalLight.hpp>
#include <ShaderManager.hpp>
#include <Scene.hpp>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// Functions prototypes
void printUsage();
void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam); 

// Window settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;
bool showMouse = false;

// Player
std::shared_ptr<Player> player;

// Wireframe
bool wireframe = false;

// Gameloop
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
float lag = 0.0f;
float MS_PER_UPDATE = 0.001;

// Trans
float PasTranslationCube = 0.01;

// Physique
double v0_Vitesse = 0.01f;
float g = 9.81;
float vitesse = 0.5;
glm::vec3 F = glm::vec3(0., 0., 0.);
glm::vec3 a = glm::vec3(0., 0., 0.);

glm::mat4 view;
glm::mat4 proj;



// Permet d'aficher ou non (métrage + timer)
bool principal = true;
bool settings = false;
bool credits = false;
bool ESCAPE;

bool toucheCPresseePrecedemment = false;
bool afficherMenu = false;

bool AZERTY = false;
bool QWERTY = true;

bool Fulscreen = false;


glm::vec3 globalPos = glm::vec3(0.);
glm::vec3 globalRot = glm::vec3(0.);

int main(int argc, char* argv[]) {
    // Initialize window
    Window window(4,1,SCR_WIDTH,SCR_HEIGHT,"Ciment doux",true);
    window.setup_GLFW();
    glfwSetKeyCallback(window.get_window(), keyCallback); 

    // // OpenGL debug
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    // glDebugMessageCallback(openglCallbackFunction, nullptr);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* font = io.Fonts->AddFontFromFileTTF("../data/fonts/BebasNeue-Regular.ttf", 65.0f);
    ImFont* fontMenu = io.Fonts->AddFontFromFileTTF("../data/fonts/BebasNeue-Regular.ttf",50.0f);
    ImFont* fontDefault = io.Fonts->AddFontDefault();


    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    glEnable(GL_DEPTH_TEST);

    // Shader
    auto shader = ShaderManager::getShader();

    // Physics engine
    auto pe = std::make_shared<PhysicsEngine>();

    // Player
    player = Player::create(window.get_window());
    pe->add_player(player);

    // Scene
    auto scene = Scene::create();
    auto plateforme = Model::create("../data/models/cube/Cube.gltf", shader);
    scene->add_entities_into_physics_engine(pe);


    auto obst2 = Model::create("../data/models/cube/Cube.gltf", shader);
    auto obst2_node = SceneNode::create(obst2);
    obst2_node->transform.set_scale(glm::vec3(2.0f,0.5f,2.f));
    obst2_node->transform.set_translation(glm::vec3(-20.f,27.f,8.6f));
    obst2_node->rigid_body->is_in_motion=true;
    pe->add_entity(obst2_node);


    auto obst3 = Model::create("../data/models/cube/Cube.gltf", shader);
    auto obst3_node = SceneNode::create(obst3);
    obst3_node->transform.set_scale(glm::vec3(15.0f,0.5f,15.f));
    obst3_node->transform.set_translation(glm::vec3(-20.f,72.f,0.f));
    pe->add_entity(obst3_node);

    // // Capsule (for test)
    // auto capsule = Model::create("../data/models/capsule/capsule.gltf", shader);
    // auto capsule_node = SceneNode::create(capsule);
    // scene->add_node(capsule_node);
    // capsule_node->set_rotation(glm::vec3(0.0f,0.0f,90.0f));
    // capsule_node->set_scale(glm::vec3(1.0f,0.5f,1.0f));


    // Variables menu
    double hauteur = 0.;
    double MaxHeight = 0.;
    int currentRun = 0;

    double timing = 0.0, acc;
    float fov = player->get_camera()->getFOV();
    float sensi = player->get_camera()->get_sensivity();


    float volume = 0.1;
    ma_result result;
    ma_engine engine;
    ma_sound sound;
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS){
        std::cout << "Failed to initialized miniaudio engine\n";
        ma_engine_uninit(&engine);
        return -1;
    }
    result = ma_sound_init_from_file(&engine, "../data/sounds/BetonBrutal3.mp3", 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS){
        std::cout << "Impossible de charger le son\n";
        ma_engine_uninit(&engine);
        return -1;
    }

    ma_sound_set_looping(&sound,true);
    ma_sound_start(&sound);

    float temps_debut = glfwGetTime();

    // Render loop
    while (glfwWindowShouldClose(window.get_window()) == 0) {
        float currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;

        if(ESCAPE == false) {
            acc += deltaTime;

            if (acc >= 1.) {
                timing += 1.0;
            }            
        } 

        ma_engine_set_volume(&engine, volume);

        // Input
        if(showMouse) {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        view = player->get_view_matrix();
        proj = player->get_projection_matrix();
        scene->draw(view, proj);
        
        pe->update(deltaTime);
        
        while (lag >= MS_PER_UPDATE) {
            if(principal == true) {
                player->update(deltaTime);
            }
            
            lag -= MS_PER_UPDATE;    
        }

        view = player->get_view_matrix();
        proj = player->get_projection_matrix(); 

        // Sending to shader

        glm::vec3 camPos = player->get_position();
        glm::vec3 camFront = player->getCFront();

        shader->useShader();
        shader->setVPMatrix(view,proj);

        // Phong + Flashlight
        // shader.setBind3f("lightPos", camPos[0], camPos[1], camPos[2]);
        shader->setBind3f("viewPos", camPos[0], camPos[1], camPos[2]);

        //mouvement plateforme
        obst2_node->transform.adjust_translation(glm::vec3(-sin(temps_debut-currentFrame)*10*deltaTime,0.f,0.f));

        if(obst2_node->rigid_body->is_child){
            //std::cout<<"-----il est lie-------"<<std::endl;
            glm::vec3 acc=obst2_node->transform.get_translation()-player->player_node->transform.get_translation();
            //player->player_node->transform.adjust_translation(glm::vec3(-sin(temps_debut-currentFrame)*10*deltaTime,0.f,0.f));
            player->player_node->transform.adjust_translation(glm::vec3(acc.x,0.f,0.f));
        }


        // Scene
        scene->draw(view, proj);        
        obst2_node->draw(view,proj);    
        //obst3_node->draw(view,proj);
        //capsule_node->draw(view, proj);
        //std::cout<<scene->scene_nodes[0]->mesh->bounding_box.min.x<<std::endl;

        char TempsFormater[9];
        char TempsFormaterMenu[9];

        if(glfwGetKey(window.get_window(), GLFW_KEY_R) == GLFW_PRESS) {
            player->player_node->transform.set_translation(glm::vec3(-21.0f, 5.0f, 23.4f));
            player->get_camera()->setRotationDegrees(glm::vec3(0., 90., 0.));
        }

        if(principal == true) {
            ImGui::SetNextWindowBgAlpha(0.5f);
            ImGui::Begin("Metrage_main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);
            //hauteur = std::floor(player->player_node->transform.get_translation().y - 3.0 * 1.54);
            hauteur = (player->player_node->transform.get_translation().y - 3.) * 1.26;
            if(hauteur >= MaxHeight) {
                MaxHeight = std::max(hauteur, MaxHeight);
            }
            
            std::cout << hauteur << std::endl;
            std::cout << MaxHeight << std::endl;

            char HauteurFormater[6];
            if((int)hauteur <= 0) {
                snprintf(HauteurFormater, sizeof(HauteurFormater), "0M");
            } else {
                snprintf(HauteurFormater, sizeof(HauteurFormater), "%.0lfM", hauteur);
            }

            ImVec2 textSize = ImGui::CalcTextSize(HauteurFormater);
            float posX_TimerMain = (ImGui::GetWindowWidth() - textSize.x) * 0.5f;
            float posY_TimerMain = (ImGui::GetWindowHeight() - textSize.y) * 0.5f;

            ImGui::SetCursorPosX(posX_TimerMain);
            ImGui::SetCursorPosY(posY_TimerMain);
            ImGui::SetWindowPos(ImVec2(0, 0));        
            ImGui::SetWindowSize(ImVec2(200, 60));  

            ImGui::Text("%s", HauteurFormater);

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();


            ImGui::SetNextWindowBgAlpha(0.5f);
            ImGui::Begin("Timer_main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1., 0., 0., 0.5));
            ImGui::PushFont(font);

            int heures = static_cast<int>(timing / 3600);
            int minutes = static_cast<int>((timing - heures * 3600) / 60);
            int secondes = static_cast<int>(timing - heures * 3600 - minutes * 60);

            snprintf(TempsFormater, sizeof(TempsFormater), "%02d:%02d:%02d", heures, minutes, secondes);

            char chaineTemps[16];
            snprintf(chaineTemps, sizeof(chaineTemps), "%.02f", timing);

            float longueurTexte = ImGui::CalcTextSize(TempsFormater).x;
            float hauteurTexte = ImGui::CalcTextSize(TempsFormater).y;

            float longueurFenetre = ImGui::GetWindowWidth();
            float hauteurFenetre = ImGui::GetWindowHeight();

            float posX_MetrageMain = (longueurFenetre - longueurTexte) * 0.5f;
            float posY_MetrageMain = (hauteurFenetre - hauteurTexte) * 0.5f;

            ImGui::SetCursorPosX(posX_MetrageMain);
            ImGui::SetCursorPosY(posY_MetrageMain);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth(), 0));      
            ImGui::SetWindowSize(ImVec2(200, 60));  

            ImGui::Text("%s", TempsFormater);

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();        
        }


        // Menu
        //ESCAPE = glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS;

        bool toucheEscapePressee = glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (toucheEscapePressee && !toucheCPresseePrecedemment) {
            ESCAPE = !ESCAPE;
        }
        toucheCPresseePrecedemment = toucheEscapePressee;


        if (ESCAPE && settings == false && credits == false) {
            globalPos = player->get_camera()->getPosition();
            player->player_node->set_translation(globalPos);
            globalRot = player->get_camera()->getRotationDegrees();
            std::cout << globalRot.x << "\t" << globalRot.y << "\t" << globalRot.z << std::endl;


            principal = false;
            showMouse = true;

            // 1er affichage
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Current_run", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);

            char RunFormater[4];
            snprintf(RunFormater, sizeof(RunFormater), "#%d", currentRun);

            ImVec2 textSizerun = ImGui::CalcTextSize(RunFormater);
            float posXrun = (ImGui::GetWindowWidth() - textSizerun.x) * 0.5f;
            float posYrun = (ImGui::GetWindowHeight() - textSizerun.y) * 0.5f;

            ImGui::SetCursorPosX(posXrun);
            ImGui::SetCursorPosY(posYrun);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, SCR_HEIGHT / 6));
            ImGui::SetWindowSize(ImVec2(1000, 175));  

            ImGui::Text("%s", RunFormater);

            ImGui::PushFont(fontMenu);
            ImGui::SetCursorPosX(5);
            ImGui::SetCursorPosY(2);
            ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.0f, 1.0f), "Current run");
            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();

            // 2ème affichage
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Curr_height", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);

            char CHFormater[6];
            if((int)hauteur <= 0) {
                snprintf(CHFormater, sizeof(CHFormater), "0M");
            } else {
                snprintf(CHFormater, sizeof(CHFormater), "%.0lfM", hauteur);
            }

            ImVec2 textSizech = ImGui::CalcTextSize(CHFormater);
            float posXcurrheight = (ImGui::GetWindowWidth() - textSizech.x) * 0.5f;
            float posYcurrheight = (ImGui::GetWindowHeight() - textSizech.y) * 0.5f;

            ImGui::SetCursorPosX(posXcurrheight);
            ImGui::SetCursorPosY(posYcurrheight);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, (SCR_HEIGHT / 6) * 2));
            ImGui::SetWindowSize(ImVec2(1000, 175));  

            ImGui::Text("%s", CHFormater);

            ImGui::PushFont(fontMenu);
            ImGui::SetCursorPosX(5);
            ImGui::SetCursorPosY(2);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Current height");
            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();

            // 3ème affichage
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Max_height", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);

            char MXFormater[6];
            if((int)MaxHeight <= 0) {
                snprintf(MXFormater, sizeof(MXFormater), "0M");
            } else {
                snprintf(MXFormater, sizeof(MXFormater), "%.0lfM", MaxHeight);
            }

            ImVec2 textSizemh = ImGui::CalcTextSize(MXFormater);
            float posXmaxheight = (ImGui::GetWindowWidth() - textSizemh.x) * 0.5f;
            float posYmaxheight = (ImGui::GetWindowHeight() - textSizemh.y) * 0.5f;

            ImGui::SetCursorPosX(posXmaxheight);
            ImGui::SetCursorPosY(posYmaxheight);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, (SCR_HEIGHT / 6) * 3));
            ImGui::SetWindowSize(ImVec2(1000, 175));  

            ImGui::Text("%s", MXFormater);

            ImGui::PushFont(fontMenu);
            ImGui::SetCursorPosX(5);
            ImGui::SetCursorPosY(2);
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.0f, 1.0f), "Max height");
            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();

            // 4ème affichage
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Timer_menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1., 0., 0., 0.5));
            ImGui::PushFont(font);

            int heures_menu = static_cast<int>(timing / 3600);
            int minutes_menu = static_cast<int>((timing - heures_menu * 3600) / 60);
            int secondes_menu = static_cast<int>(timing - heures_menu * 3600 - minutes_menu * 60);

            snprintf(TempsFormaterMenu, sizeof(TempsFormaterMenu), "%02d:%02d:%02d", heures_menu, minutes_menu, secondes_menu);

            char chaineTempsMenu[16];
            snprintf(chaineTempsMenu, sizeof(chaineTempsMenu), "%.02f", timing);

            float longueurTexte = ImGui::CalcTextSize(TempsFormaterMenu).x;
            float hauteurTexte = ImGui::CalcTextSize(TempsFormaterMenu).y;

            float longueurFenetre = ImGui::GetWindowWidth();
            float hauteurFenetre = ImGui::GetWindowHeight();

            float posX_MetrageMain = (longueurFenetre - longueurTexte) * 0.5f;
            float posY_MetrageMain = (hauteurFenetre - hauteurTexte) * 0.5f;

            ImGui::SetCursorPosX(posX_MetrageMain);
            ImGui::SetCursorPosY(posY_MetrageMain);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, (SCR_HEIGHT / 6) * 4));
            ImGui::SetWindowSize(ImVec2(1000, 175));  

            ImGui::Text("%s", TempsFormaterMenu);


            ImGui::PushFont(fontMenu);
            ImGui::SetCursorPosX(5);
            ImGui::SetCursorPosY(2);
            ImGui::TextColored(ImVec4(0.0f, 0.6f, 1.0f, 1.0f), "Current time");
            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End(); 


            // Paramètres
            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Play", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Play").x) * 0.5f;
            float yPos = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Play").y) * 0.5f;
            ImGui::SetCursorPosX(xPos);
            ImGui::SetCursorPosY(yPos);
            ImGui::SetWindowPos(ImVec2(5, SCR_HEIGHT / 6));        
            ImGui::SetWindowSize(ImVec2(205, 55));  

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Play"))
            {
                ESCAPE = false;
                globalPos = player->get_camera()->getPosition();
                player->player_node->set_translation(globalPos);

                player->get_camera()->setRotationDegrees(globalRot);
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();

    
            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Restart run", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos_res = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Restart run").x) * 0.5f;
            float yPos_res = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Restart run").y) * 0.5f;
            ImGui::SetCursorPosX(xPos_res);
            ImGui::SetCursorPosY(yPos_res);
            ImGui::SetWindowPos(ImVec2(5, (SCR_HEIGHT / 6) * 1.35));            
            ImGui::SetWindowSize(ImVec2(205, 55));  

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Restart run"))
            {
                currentRun += 1;
                MaxHeight = std::min(0., hauteur);
                player->player_node->transform.set_translation(glm::vec3(-21.0f, 5.0f, 23.4f));

                player->get_camera()->setRotationDegrees(glm::vec3(0., 90., 0.));

                timing = 0.;
                acc = 0.0;
                ESCAPE = false;
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();


            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos_set = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Settings").x) * 0.5f;
            float yPos_set = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Settings").y) * 0.5f;
            ImGui::SetCursorPosX(xPos_set);
            ImGui::SetCursorPosY(yPos_set);
            ImGui::SetWindowPos(ImVec2(5, (SCR_HEIGHT / 6) * 1.7));        
            ImGui::SetWindowSize(ImVec2(205, 55));  
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Settings"))
            {
                settings = true;
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();


            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Credits", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos_cred = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Credits").x) * 0.5f;
            float yPos_cred = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Credits").y) * 0.5f;
            ImGui::SetCursorPosX(xPos_cred);
            ImGui::SetCursorPosY(yPos_cred);
            ImGui::SetWindowPos(ImVec2(5, (SCR_HEIGHT / 6) * 2.05));        
            ImGui::SetWindowSize(ImVec2(205, 55));  
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Credits"))
            {
                credits = true;
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();


            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Exit", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos_exit = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Exit").x) * 0.5f;
            float yPos_exit = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Exit").y) * 0.5f;
            ImGui::SetCursorPosX(xPos_exit);
            ImGui::SetCursorPosY(yPos_exit);
            ImGui::SetWindowPos(ImVec2(5, (SCR_HEIGHT / 6) * 2.4));        
            ImGui::SetWindowSize(ImVec2(205, 55));  
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Exit"))
            {
                glfwTerminate();
                exit(0);
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();
        } else if(ESCAPE == true && settings == true) {
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Slides", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 12, SCR_HEIGHT / 10));
            ImGui::SetWindowSize(ImVec2(SCR_WIDTH / 2 + SCR_WIDTH / 3, SCR_HEIGHT / 1.25));  

            float contentWidth = ImGui::GetContentRegionAvail().x;
            float contentHeight = ImGui::GetContentRegionAvail().y;

            float textHeight = ImGui::GetTextLineHeightWithSpacing(); 
            float posY = (contentHeight - textHeight * 12) * 0.5f;

            float sliderWidth = 200.0f;
            float textWidthFOV = ImGui::CalcTextSize("FOV").x;
            float textWidthSensi = ImGui::CalcTextSize("Sensibilité de la souris").x;
            float textWidthVolume = ImGui::CalcTextSize("Volume du son").x;
            float textWidthCom = ImGui::CalcTextSize("Commandes AZERTY / QWERTY :").x;
            float textWidthFullscreen = ImGui::CalcTextSize("Plein écran :").x;

            float textPosXFOV = (contentWidth - textWidthFOV) * 0.5f;
            float textPosXSensi = (contentWidth - textWidthSensi) * 0.5f;
            float textPosXVolume = (contentWidth - textWidthVolume) * 0.5f;
            float textPosXCom = (contentWidth - textWidthCom) * 0.5f;
            float textPosXFullscreen = (contentWidth - textWidthFullscreen) * 0.5f;

            float sliderPosX = (contentWidth - sliderWidth) * 0.5f;

            ImGui::SetCursorPosX(textPosXFOV);
            ImGui::SetCursorPosY(posY);
            ImGui::Text("FOV");

            float imguiWindowSize = ImGui::GetWindowSize().x;
            float sliderPosOffsetFOV = imguiWindowSize / 4;
            float sliderPosXFOV = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetFOV;
            ImGui::SetCursorPosX(sliderPosXFOV);
            ImGui::SliderFloat("##FOV", &fov, 45.f, 120.f);
            player->get_camera()->setFOV(fov);

            ImGui::SetCursorPosX(textPosXSensi);
            ImGui::SetCursorPosY(posY + textHeight + 100);
            ImGui::Text("Sensibilité de la souris");

            float imguiWindowSizee = ImGui::GetWindowSize().x;
            float sliderPosOffsetSensi = imguiWindowSizee / 4;
            float sliderPosXSensi = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetSensi;
            ImGui::SetCursorPosX(sliderPosXSensi);;
            ImGui::SliderFloat("##Sensibilite", &sensi, 0.01f, 10.f);
            player->get_camera()->set_sensitivity(sensi);

            ImGui::SetCursorPosX(textPosXVolume);
            ImGui::SetCursorPosY(posY + textHeight + 270);
            ImGui::Text("Volume du son");

            float imguiWindowSizeee = ImGui::GetWindowSize().x;
            float sliderPosOffsetVolume = imguiWindowSizeee / 4;
            float sliderPosXVolume = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetVolume;
            ImGui::SetCursorPosX(sliderPosXVolume);;
            ImGui::SliderFloat("##Volume", &volume, 0.0f, 1.f);

            ImGui::SetCursorPosX(textPosXCom);
            ImGui::SetCursorPosY(posY + textHeight + 450);
            ImGui::Text("Commandes AZERTY / QWERTY :");

            float imguiWindowSizeeee = ImGui::GetWindowSize().x;
            float sliderPosOffsetCom = imguiWindowSizeeee / 4;
            float sliderPosXCom = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetCom;
            float buttonWidth = 25;
            float totalButtonWidth = 2 * buttonWidth;
            float spaceBetweenButtons = imguiWindowSizeeee - totalButtonWidth;
            float offsetX = (SCR_WIDTH / 4); 
            ImGui::SetCursorPosX(offsetX);
            if(ImGui::Checkbox("AZERTY", &AZERTY)) {
                player->get_camera()->mode_cam = 1;
                QWERTY = false;
            }
            ImGui::SameLine();
            if(ImGui::Checkbox("QWERTY", &QWERTY)) {
                player->get_camera()->mode_cam = 0;
                AZERTY = false;
            }

            ImGui::SetCursorPosX(textPosXFullscreen);
            ImGui::SetCursorPosY(posY + textHeight + 625);
            ImGui::Text("Plein écran :");

            float checkboxWidth = ImGui::CalcTextSize("").x;
            float offsetXFSC = (SCR_WIDTH - checkboxWidth) / 2.65;
            ImGui::SetCursorPosX(offsetXFSC);
            ImGui::Checkbox("##", &Fulscreen);
            ImGui::End();

        } else if(ESCAPE == true && credits == true) {
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Creditss", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 9.2, SCR_HEIGHT / 8));
            ImGui::SetWindowSize(ImVec2(SCR_WIDTH / 2 + SCR_WIDTH / 3 - 80, 800));  

            char CHFormater1[] = "SERVA Benjamin";
            char CHFormater2[] = "JEAN Louis";
            char CHFormater3[] = "COMBOT Evan";
            char CHFormater4[] = "RONTEIX Jonas";

            ImVec2 textSizech1 = ImGui::CalcTextSize(CHFormater1);
            ImVec2 textSizech2 = ImGui::CalcTextSize(CHFormater2);
            ImVec2 textSizech3 = ImGui::CalcTextSize(CHFormater3);
            ImVec2 textSizech4 = ImGui::CalcTextSize(CHFormater4);

            float posX1 = (ImGui::GetWindowWidth() - textSizech1.x) * 0.5f;
            float posY1 = (ImGui::GetWindowHeight() - 3 * textSizech1.y) * 0.5f;

            float posX2 = (ImGui::GetWindowWidth() - textSizech2.x) * 0.5f;
            float posY2 = (ImGui::GetWindowHeight() - textSizech2.y) * 0.5f;

            float posX3 = (ImGui::GetWindowWidth() - textSizech3.x) * 0.5f;
            float posY3 = (ImGui::GetWindowHeight() + textSizech3.y) * 0.5f;

            float posX4 = (ImGui::GetWindowWidth() - textSizech4.x) * 0.5f;
            float posY4 = (ImGui::GetWindowHeight() + 3 * textSizech4.y) * 0.5f;

            ImGui::SetCursorPosX(posX1);
            ImGui::SetCursorPosY(posY1);
            ImGui::Text("%s", CHFormater1);

            ImGui::SetCursorPosX(posX2);
            ImGui::SetCursorPosY(posY2);
            ImGui::Text("%s", CHFormater2);

            ImGui::SetCursorPosX(posX3);
            ImGui::SetCursorPosY(posY3);
            ImGui::Text("%s", CHFormater3);

            ImGui::SetCursorPosX(posX4);
            ImGui::SetCursorPosY(posY4);
            ImGui::Text("%s", CHFormater4);

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();
        } else {
            principal = true; 
            showMouse = false;  
            settings = false;    
            credits = false;    
        }


        // Render window & ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        glfwSwapBuffers(window.get_window());
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_Z && player->get_camera()->mode_cam!=0) {
            wireframe = !wireframe;
            if(wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        if(key == GLFW_KEY_A && player->get_camera()->mode_cam==0) {
            wireframe = !wireframe;
            if(wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        if(key == GLFW_KEY_T) {
            showMouse = player->get_camera()->getShowMouse();
            showMouse = !showMouse;
            player->get_camera()->setShowMouse(showMouse);
        }
        if (key == GLFW_KEY_H) {
            player->get_camera()->mode_cam=(player->get_camera()->mode_cam+1)%3;
            player->get_camera()->reset();
        }
    }
    //player->handleSingleInput(key, scancode, action, mods);
}


void processInput(GLFWwindow *window) {
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void APIENTRY openglCallbackFunction(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam) {

    (void)source; (void)type; (void)id; 
    (void)severity; (void)length; (void)userParam;
    fprintf(stderr, "%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        fprintf(stderr, "Aborting...\n");
        abort();
    }
}