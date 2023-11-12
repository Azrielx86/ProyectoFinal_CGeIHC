#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Animation/Animation.h"
#include "Audio/AudioDevice.h"
#include "GlobalConstants.h"
#include "Lights/DirectionalLight.h"
#include "Lights/LightCollection.h"
#include "Skybox.h"
#include "Utils/ModelMatrix.h"
#include "Window.h"
#include "camera/Camera.h"
#include "camera/CameraCollection.h"
#include "input/KeyboardInput.h"
#include "model/BasicPrimitives.h"
#include "model/Material.h"
#include "model/Model.h"
#include "model/ModelCollection.h"

Window mainWindow;
Camera::CameraCollection cameras;
Camera::Camera *activeCamera;
Model::ModelCollection models;
Lights::LightCollection<Lights::DirectionalLight> directionalLights;
Lights::LightCollection<Lights::PointLight> pointLights;
Lights::LightCollection<Lights::SpotLight> spotLights;
Skybox skybox;

Model::Material Material_brillante;
Model::Material Material_opaco;

std::unordered_map<int, Shader *> shaders;

float deltaTime = 0.0f;
float lastTime = 0.0f;
const float limitFPS = 1.0f / 60.0f;

AMB_LIGHTS ambLight = AMB_LIGHTS::DAY;

// Variables auxiliares que no se encapsularon en otras clases :v
float rightFlipperRotation = 0.0f;
float leftFlipperRotation = 0.0f;

void InitKeymaps()
{
	Input::KeyboardInput::GetInstance()
	    .createKeymap(KEYMAPS::FREE_CAMERA)
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_ESCAPE,
	        []() -> void
	        {
		        glfwSetWindowShouldClose(mainWindow.getWindowPointer(), GL_TRUE);
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_C,
	        []() -> void
	        {
		        activeCamera = cameras.switchCamera();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_T,
	        []() -> void
	        {
		        std::cout << "Mouse disabled!";
		        Input::MouseInput::GetInstance().toggleMouseEnabled();
		        mainWindow.toggleMouse();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_P,
	        []() -> void
	        {
		        std::cout << "Tecla P presionada\n";
	        }, true)
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_L,
	        []() -> void
	        {
		        if (ambLight == AMB_LIGHTS::DAY)
			        ambLight = AMB_LIGHTS::NIGHT;
		        else
			        ambLight = AMB_LIGHTS::DAY;
	        });
	/*.addCallback(
			 KEYMAPS::FREE_CAMERA, GLFW_KEY_U,
			 []() -> void
			  {
				//pointLights.toggleLignt(ind:2, new state: !pointLights.getLightStauts(2);
			 })*/

	//	    .addCallback(
	//	        KEYMAPS::FREE_CAMERA,
	//	        GLFW_KEY_L,
	//	        []() -> void
	//	        {
	//		        std::cout << "Flipper Right: " << rightFlipperRotation << '\n';
	//		        if (rightFlipperRotation < -25)
	//			        return;
	//		        rightFlipperRotation -= 8;
	//	        },
	//	        true);

	Input::MouseInput::GetInstance()
	    .createKeymap(KEYMAPS::FREE_CAMERA)
	    .addClickCallback(
	        KEYMAPS::FREE_CAMERA,
	        GLFW_MOUSE_BUTTON_LEFT,
	        []() -> void
	        {
		        std::cout << "Click izquierdo presionado\n";
	        })
	    .addClickCallback(
	        KEYMAPS::FREE_CAMERA,
	        GLFW_MOUSE_BUTTON_RIGHT,
	        []() -> void
	        {
				std::cout << "Click derecho presionado\n";
			})
			.addMoveCallback(
				KEYMAPS::FREE_CAMERA,
				[](float) -> void
				{
					activeCamera->mouseControl(Input::MouseInput::GetInstance());
				});
}

void InitShaders()
{
	auto shader = new Shader();
	shader->loadShader("shaders/shader.vert", "shaders/shader.frag");
	shaders[Shader::ShaderTypes::BASE_SHADER] = shader;

	auto lightShader = new Shader();
	lightShader->loadShader("shaders/shader_light.vert", "shaders/shader_light.frag");
	shaders[Shader::ShaderTypes::LIGHT_SHADER] = lightShader;
}

void InitCameras()
{
	cameras.addCamera(new Camera::Camera(glm::vec3(0.0f, 60.0f, 20.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-60.0f, 0.0f, 0.5f, 0.5f));
	cameras.addCamera(new Camera::Camera(glm::vec3(-134.618, 124.889, 4.39917),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f, -30.0f, 0.5f, 0.5f, true));
	activeCamera = cameras.getAcviveCamera();
}

void InitModels()
{
	models
		.addModel(MODELS::MAQUINA_PINBALL, "assets/Models/MaquinaPinball.obj")
		.addModel(MODELS::FLIPPER, "assets/Models/Flipper.obj")
		.addModel(MODELS::CANICA, "assets/Models/canica.obj")
		.addModel(MODELS::NORA, "assets/Models/NoraF.obj")
		.addModel(MODELS::MUNECO, "assets/Models/muneco_Hielo.obj")
		.addModel(MODELS::PICOP, "assets/Models/Pico_P.obj")
		.addModel(MODELS::PICOM, "assets/Models/Pico_M.obj")
		.addModel(MODELS::PICOG, "assets/Models/Pico_G.obj")
	    .addModel(MODELS::BASE, "assets/Models/basePinball.obj")
		.addModel(MODELS::MAQUINA_CRISTAL, Utils::PathUtils::getModelsPath().append("/MaquinaCristal.obj"))
		.loadModels();
}

void InitLights()
{
	Lights::LightCollectionBuilder<Lights::DirectionalLight> directionalLightsBuilder(2);
	directionalLights = directionalLightsBuilder
		.addLight(Lights::DirectionalLight(1.0f, 1.0f, 1.0f,
			0.8f, 0.3f,
			0.0f, -1.0f, 0.0f))
		.addLight(Lights::DirectionalLight(1.0f, 1.0f, 1.0f,
			0.3f, 0.3f,
			0.0f, 0.0f, -1.0f))
		.build();

	Lights::LightCollectionBuilder<Lights::PointLight> pointLightsBuilder(3);
	pointLights = pointLightsBuilder
		.addLight(Lights::PointLight(1.0f, 0.0f, 1.0f,
			0.8f, 0.3f,
			0.0f, 0.0f, 0.0f,
			1.0f, 0.01f, 0.001f))
		.addLight(Lights::PointLight(0.0f, 0.0f, 1.0f,//1er pico
	        0.8f, 0.3f,
			15.5f, 60.35f, -10.0f,
	        1.0f, 0.01f, 0.001f))
		.addLight(Lights::PointLight(0.0f, 0.0f, 1.0f,//2dopico
	        0.8f, 0.3f,
	        31.0f, 64.1f, -26.0f,
	        1.0f, 0.01f, 0.001f))
	    .addLight(Lights::PointLight(0.0f, 0.0f, 1.0f,//3erpico
	        0.8f, 0.3f,
	        6.5f, 59.35f, 6.0f,
	        1.0f, 0.01f, 0.001f))
		.build();
	Lights::LightCollectionBuilder<Lights::SpotLight> spotLightBuilder(3);
	spotLights = spotLightBuilder
	  /*.addLight(Lights::SpotLight(0.0f, 0.0f, 1.0f,
	        0.8f, 0.3f,
	        0.0f, 0.0f, 0.0f,//pos
			1.0f, 0.01f, 0.001f,//dir
			1.0f, 0.01f, 0.0f,//ec y angulo
			1.0f))*/
		.build();
}

void updateFlippers()
{
	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_RIGHT_SHIFT).pressed)
	{
		if (rightFlipperRotation > -20)
			rightFlipperRotation -= 5;
	}
	else
	{
		if (rightFlipperRotation < 20)
			rightFlipperRotation += 5;
	}

	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_LEFT_SHIFT).pressed)
	{
		if (leftFlipperRotation < 20)
			leftFlipperRotation += 5;
	}
	else
	{
		if (leftFlipperRotation > -20)
			leftFlipperRotation -= 5;
	}
	//prueba animaciones
	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_O).pressed)
	{
		mainWindow.setStartAnimacionPico1TRUE();
	}
	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_I).pressed)
	{
		mainWindow.setStartAnimacionPico2TRUE();
	}
	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_P).pressed)
	{
		mainWindow.setStartAnimacionPico3TRUE();
	}
	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_U).pressed)
	{
		mainWindow.setStartAnimacionCanicaTRUE();
	}
}

int main()
{
	mainWindow = Window(1280, 720, "Proyecto Final \"Maquina de pinball\" - Semestre 2024-1");

	if (!mainWindow.Init())
	{
		std::cerr << "No se pudo iniciar la ventana\n";
		return 1;
	}

	// Inicializar los componentes del programa
	InitShaders();
	InitKeymaps();
	InitCameras();
	InitModels();
	InitLights();

	std::vector<std::string> skyboxFaces;

	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_rt.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_lf.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_dn.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_up.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_bk.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_ft.png");

	skybox = Skybox(skyboxFaces);

	Audio::AudioDevice::GetInstance();

	Material_brillante = Model::Material(4.0f, 256);
	Material_opaco = Model::Material(0.3f, 4);

	// Constantes para uniforms
	GLuint uProjection, uModel, uView, uEyePosition, uSpecularIntensity, uShininess, uTexOffset, uColor;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / (GLfloat) mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	Utils::ModelMatrix handler(glm::mat4(1.0f));
	glm::mat4 model(1.0f);
	glm::mat4 aux(1.0f);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::mat4 modelaux3(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	
	//variables para animacion
	float rotOffPico=3.5;
	float escOffPico = .01;
	float rotPico1=0;
	float rotPicoZ1 = 0;
	float escPico1 = 0;
	float rotPico2 = 0;
	float rotPicoZ2 = 0;
	float escPico2 = 0;
	float rotPico3 = 0;
	float rotPicoZ3 = 0;
	float escPico3 = 0;
	
	float movXCanica = 0;
	float movYCanica = 0;
	float movZCanica = 0;
	float movXOffCanica = 0.4;
	float movYOffCanica = 0.06;
	float movZOffCanica = 0.2;
	float movYImpulso = 0.0524;
	float movXImpulso = 0.5;
	float rotXCanica = 0.0;
	float rotZCanica = 0.0;
	float rotOffCanica = 1.5;
	int BCan = 0;//bandera canica

	float XN =0;
	float YN =0; 
	float ZN =0;
	float ZA = 0;
	float YA = 0;
	float XA = 0;
	float ZAn = 0;
	float YAn = 0;
	float XAn = 0;
	float t = 0;
	float i = 0;
	// modelos
	auto maquinaPinball = models[MODELS::MAQUINA_PINBALL];
	auto flipper = models[MODELS::FLIPPER];
	auto Nora = models[MODELS::NORA];
	auto Muneco = models[MODELS::MUNECO];
	auto PicoP = models[MODELS::PICOP];
	auto PicoM = models[MODELS::PICOM];
	auto PicoG = models[MODELS::PICOG];
	auto Canica = models[MODELS::CANICA];
	auto Base = models[MODELS::BASE];//
	// Shaders
	auto shaderLight = shaders[Shader::ShaderTypes::LIGHT_SHADER];

	//animaciones
	Animation CanicaAS;
	CanicaAS
	    .addCondition(//activar la palanca del resorte y dejar presionado
	        [](float delta) -> bool
	        { return mainWindow.getStartAnimacionCanica(); })
	    .addCondition(
	        [&movZCanica, &movZOffCanica, &rotXCanica, &rotOffCanica, &BCan](float delta) -> bool
	        {
				if (BCan==0) {
					if (movZCanica < 29 )
					{
						movZCanica += movZOffCanica * delta;
						rotXCanica += rotOffCanica * delta;
						return false;
			        }
					else {
				        BCan = 1;
				        return false;
					}
				}
			    else
				    return true; })
		.addCondition(//soltar palanca pendiente
	        [](float delta) -> bool
	        {return mainWindow.getStartAnimacionCanica(); })
		.addCondition(//llega a la parte de arriba
	        [&movZCanica, &movZOffCanica, &movXCanica, &movYCanica, &movXImpulso, &movYImpulso, &rotZCanica, &rotXCanica, &rotOffCanica, &BCan](float delta) -> bool
			{
				if (movXCanica < 100 && BCan==1)
				{
			        movXCanica += movXImpulso * delta;
			        movYCanica += movYImpulso * delta;
					rotZCanica += rotOffCanica *delta;
			        movZOffCanica = 0.23;
					return false;
				}
		        else if (movXCanica > 100 && movXCanica < 125 && movZCanica > 19 && BCan == 1)
		        {
			        movXCanica += movXImpulso * delta;
			        movZCanica -= movZOffCanica * delta;
			        movYCanica += movYImpulso * delta;
			        rotZCanica += rotOffCanica * delta;
			        return false;
				}
				else
					BCan=2;
					movZOffCanica = 0.38;
					return true; })
	    .addCondition(
	        [&movXCanica, &movXOffCanica, &movYCanica, &movYOffCanica, &movZCanica, &movZOffCanica, &rotXCanica, &rotZCanica, &rotOffCanica, &BCan](float delta) -> bool 
			{
				if (movZCanica > 12 && BCan==2) {
			        movXCanica += 0.06 * delta;
			        movZCanica -= movZOffCanica *.5 * delta;
					if (movYCanica > 57) {
						movYCanica -= 0.08 * delta;
					}
					rotXCanica += rotOffCanica * delta;
		        }
		        else if (movXCanica > 106 && BCan == 2)
		        {
					movXCanica -= movXOffCanica * delta;
			        movZCanica -= movZOffCanica * delta;
			        if (movYCanica > 11)
			        {
				        movYCanica -= 0.08 * delta;
			        }
			        rotZCanica += rotOffCanica * delta;
				}
				else
			        BCan=3;
					return true; })
	    .addCondition(
	        [&rotZCanica, &rotOffCanica, &movXCanica, &movXOffCanica, &movYCanica, &movYOffCanica, &movZCanica, &movZOffCanica, &BCan](float delta) -> bool
	        {
				movXOffCanica=0.7;
				if (BCan==3) {
					if (movYCanica > 10)
			        {
				        movYCanica -= 0.1 * delta;
			        }
					if (movXCanica > 95 ) {
						movXCanica -= movXOffCanica * delta;
						movZCanica -= movZOffCanica *.8 * delta;
						rotZCanica -= rotOffCanica * delta;
					}else if (movXCanica > 90 && movXCanica < 95) {
				        movXOffCanica = 1.0;
				        movXCanica -= movXOffCanica * delta;
			        }
			        else
				        BCan = 4; 
		        }
				else
					return true; })
	    .addCondition( // activa pico
	        [&i,&BCan](float delta) -> bool
				{ 
				if (BCan == 4) {
					if (i<3){
					i += 12.0 * delta;
					printf("i es: %f \n", i);
			        }
					else {
						i = 10;
						mainWindow.setStartAnimacionPico3TRUE();
						BCan = 5;
				        printf("paso por A \n");
					}
				}
		        else if (BCan==5)
				{
			        printf("paso por b \n");
			        return true;
				}
				return mainWindow.getStartAnimacionCanica(); })
	    .addCondition(
	        [&XN, &YN, &ZN, &XA, &YA, &ZA, &XAn, &YAn, &ZAn, &BCan, &t](float delta) -> bool
	        {
		        if (BCan == 5)
		        {
			        printf("paso por c \n");
					XN = 20.0;
					YN = 56.0;
				    ZN = -5.0;
				    XAn = 12.0;
				    YAn = 55.35;
				    ZAn = 4;
			        BCan = 6;
				}
		        else if (t < 1 &&BCan==6)
		        {
			        //i = 1;
			        XA =  ((1 - t) * XAn + t * XN);//78.0
					YA =  ((1 - t) * YAn + t * YN);//-45.5
					ZA =  ((1 - t) * ZAn + t * ZN);//-7.5
			        t += 0.1;
			        printf("%f\n", t);
			        printf("%f, %f, %f \n", XA,YA,ZA);
				}
		        else
					return true;
	        })
		.prepare();

	Animation PicoJerarquia1;
	PicoJerarquia1
		.addCondition(
	     [](float delta) -> bool
		 { return mainWindow.getStartAnimacionPico1(); })
		.addCondition(
	        [&rotOffPico, &rotPico1, &rotPicoZ1, &escOffPico, &escPico1](float delta) -> bool
	        {
				pointLights.toggleLight(1,true);//luces se prende
				if (rotPico1 < 720)
			    {
			        if (rotPicoZ1 < 6 && rotPico1<70)
			        {
				        rotPicoZ1 +=10* escOffPico * delta;
					}
				    rotPico1 += rotOffPico * delta;
					if (escPico1 < 2.2) {
				        escPico1 += escOffPico * delta;
					}
			        if (rotPicoZ1 > 6 && rotPico1 > 650)
			        {
				        rotPicoZ1 -= 10 * escOffPico * delta;
			        }

				    return false;
			    }
			    else
			        rotPicoZ1 = 0;
				    return true;}
		)
		.prepare();
	Animation PicoJerarquia2;
	PicoJerarquia2
	    .addCondition(
	        [](float delta) -> bool
	        { return mainWindow.getStartAnimacionPico2(); })
	    .addCondition(
	        [&rotOffPico, &rotPico2, &rotPicoZ2, &escOffPico, &escPico2](float delta) -> bool
	        {
				pointLights.toggleLight(2,true);//luces se prende
				if (rotPico2 < 720)
			    {
			        if (rotPicoZ2 < 6 && rotPico2<70)
			        {
				        rotPicoZ2 +=10* escOffPico * delta;
					}
				    rotPico2 += rotOffPico * delta;
					if (escPico2 < 2.2) {
				        escPico2 += escOffPico * delta;
					}
			        if (rotPicoZ2 > 6 && rotPico2 > 650)
			        {
				        rotPicoZ2 -= 10 * escOffPico * delta;
			        }

				    return false;
			    }
			    else
			        rotPicoZ2 = 0;
				    return true; })
	    .prepare();

	Animation PicoJerarquia3;
	PicoJerarquia3
	    .addCondition(
	        [](float delta) -> bool
	        { return mainWindow.getStartAnimacionPico3(); })
	    .addCondition(
	        [&rotOffPico, &rotPico3, &rotPicoZ3, &escOffPico, &escPico3](float delta) -> bool
	        {
				pointLights.toggleLight(3,true);//luces se prende
				if (rotPico3 < 720)
			    {
			        if (rotPicoZ3 < 6 && rotPico3<70)
			        {
				        rotPicoZ3 +=10* escOffPico * delta;
					}
				    rotPico3 += rotOffPico * delta;
					if (escPico3 < 2.2) {
				        escPico3 += escOffPico * delta;
					}
			        if (rotPicoZ3 > 6 && rotPico3 > 650)
			        {
				        rotPicoZ3 -= 10 * escOffPico * delta;
			        }

				    return false;
			    }
			    else
			        rotPicoZ3 = 0;
				    return true; })
	    .prepare();

	while (!mainWindow.shouldClose())
	{
		auto now = (float) glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		
		glfwPollEvents();
		activeCamera->keyControl(Input::KeyboardInput::GetInstance(), deltaTime);

		updateFlippers();
		if (mainWindow.getStartAnimacionPico1() == false) {
			pointLights.toggleLight(1,false);//luces apagadas de los picos
		}
		if (mainWindow.getStartAnimacionPico2() == false)
		{
			pointLights.toggleLight(2, false); // luces apagadas de los picos
		}
		if (mainWindow.getStartAnimacionPico3() == false)
		{
			pointLights.toggleLight(3, false); // luces apagadas de los picos
		}
		PicoJerarquia1.update(deltaTime);
		PicoJerarquia2.update(deltaTime);
		PicoJerarquia3.update(deltaTime);
		CanicaAS.update(deltaTime);

		// Configuración del shader
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(activeCamera->calculateViewMatrix(), projection);
		shaderLight->useProgram();
		uModel = shaderLight->getUniformModel();
		uProjection = shaderLight->getUniformProjection();
		uView = shaderLight->getUniformView();
		uEyePosition = shaderLight->getUniformEyePosition();
		uColor = shaderLight->getUniformColor();
		uTexOffset = shaderLight->getUniformTextureOffset();
		uSpecularIntensity = shaderLight->getUniformSpecularIntensity();
		uShininess = shaderLight->getUniformShininess();

		// Camara
		glUniformMatrix4fv((GLint) uProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv((GLint) uView, 1, GL_FALSE, glm::value_ptr(activeCamera->calculateViewMatrix()));
		auto camPos = activeCamera->getCameraPosition();
		glUniform3f((GLint) uEyePosition, camPos.x, camPos.y, camPos.z);

		// Iluminacion
		shaderLight->SetDirectionalLight(&directionalLights[ambLight]);
		shaderLight->SetSpotLights(nullptr, 0);
		shaderLight->SetPointLights(pointLights.getLightArray(), pointLights.getCurrentCount());
		Material_opaco.UseMaterial(uSpecularIntensity, uShininess);

		toffset = {0.0f, 0.0f};
		color = {1.0f, 1.0f, 1.0f};

		glUniform2fv((GLint) uTexOffset, 1, glm::value_ptr(toffset));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(0.0, -1.0, 0.0)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));
		maquinaPinball.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-58, 48, 10)
		            .rotateZ(6)
		            .rotateY(rightFlipperRotation)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper.render();
		
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-58, 48, -19)
		            .rotateZ(-6)
		            .rotateY(180 + leftFlipperRotation)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper.render();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(26, 61, 19)
		            .scale(2.0)
		            .rotateY(-90)
		            .rotateX(6)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		Nora.render();
		glDisable(GL_BLEND);

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(8, 55, -26)
		            .scale(0.4)
		            .rotateZ(-6)
		            .rotateY(180)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		Muneco.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(0, -90, 00)
		            .scale(30.0)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		Base.render();

		Material_brillante.UseMaterial(uSpecularIntensity, uShininess);
		//canica animada simple
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-78.0 + movXCanica + XA, 45.5 + movYCanica+YA, 7.5 + movZCanica+ZA)
		            .rotateX(rotXCanica)
		            .rotateZ(6+rotZCanica)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		Canica.render();

		//1er pico animado
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(15.5, 57.35, -10)
		            .rotateY(rotPico1)
		            .saveActualState(modelaux)// modelaux = model;   
		            .scale(2.2)
		            .rotateZ(6 - rotPicoZ1)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoG.render();
		//1
		model = handler.setMatrix(modelaux)
		            .translate(-2.5, 1.55, 0)
		            .scale(escPico1)
		            .rotateZ(44.39)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//2
		model = handler.setMatrix(modelaux)
		            .translate(.5, 3.55, +2)
					.rotateZ(-8)
		            .rotateX(48)
		            .scale(escPico1)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//3
		model = handler.setMatrix(modelaux)
		            .translate(.5, 3.55, -2)
		            .rotateZ(-8)
		            .rotateX(-48)
		            .scale(escPico1)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//4
		model = handler.setMatrix(modelaux)
		            .translate(-0.7, 5.55, +.5)
		            .scale(escPico1)
		            .rotateZ(40)
		            .rotateX(24)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//5
		model = handler.setMatrix(modelaux)
		            .translate(-0.5, 5.55, -.5)
		            .scale(escPico1)
		            .rotateZ(25)
		            .rotateX(-20)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//2do pico
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(31, 61.1, -26)
		            .rotateY(rotPico2)
		            .saveActualState(modelaux2) // modelaux = model;
		            .scale(2.2)
		            .rotateZ(6 - rotPicoZ2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoG.render();
		// 1
		model = handler.setMatrix(modelaux2)
		            .translate(-2.5, 1.55, 0)
		            .scale(escPico2)
		            .rotateZ(44.39)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 2
		model = handler.setMatrix(modelaux2)
		            .translate(.5, 3.55, +2)
		            .rotateZ(-8)
		            .rotateX(48)
		            .scale(escPico2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 3
		model = handler.setMatrix(modelaux2)
		            .translate(.5, 3.55, -2)
		            .rotateZ(-8)
		            .rotateX(-48)
		            .scale(escPico2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 4
		model = handler.setMatrix(modelaux2)
		            .translate(-0.7, 5.55, +.5)
		            .scale(escPico2)
		            .rotateZ(40)
		            .rotateX(24)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 5
		model = handler.setMatrix(modelaux2)
		            .translate(-0.5, 5.55, -.5)
		            .scale(escPico2)
		            .rotateZ(25)
		            .rotateX(-20)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();

		// 3do pico
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(6.5, 56.35, 6)
		            .rotateY(rotPico3)
		            .saveActualState(modelaux3) // modelaux = model;
		            .scale(2.2)
		            .rotateZ(6 - rotPicoZ3)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoG.render();
		// 1
		model = handler.setMatrix(modelaux3)
		            .translate(-2.5, 1.55, 0)
		            .scale(escPico3)
		            .rotateZ(44.39)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 2
		model = handler.setMatrix(modelaux3)
		            .translate(.5, 3.55, +2)
		            .rotateZ(-8)
		            .rotateX(48)
		            .scale(escPico3)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 3
		model = handler.setMatrix(modelaux3)
		            .translate(.5, 3.55, -2)
		            .rotateZ(-8)
		            .rotateX(-48)
		            .scale(escPico3)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 4
		model = handler.setMatrix(modelaux3)
		            .translate(-0.7, 5.55, +.5)
		            .scale(escPico3)
		            .rotateZ(40)
		            .rotateX(24)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 5
		model = handler.setMatrix(modelaux3)
		            .translate(-0.5, 5.55, -.5)
		            .scale(escPico3)
		            .rotateZ(25)
		            .rotateX(-20)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	Audio::AudioDevice::Terminate();

	return 0;
}
#pragma clang diagnostic pop