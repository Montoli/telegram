#include "main_state.h"
#include <SDL.h>
#include <stdio.h>
#include "GL/glew.h"


MainState::MainState(SDL_Window* window, SDL_Surface* screen_surface,
  SDL_GLContext context, int screen_width, int screen_height) {
  CommonComponent* common_data = common_system_.CommonData();
  common_data->window = window;
  common_data->screen_surface = screen_surface;
  common_data->gl_context = context;
  common_data->screen_size = vec2(static_cast<float>(screen_width),
                            static_cast<float>(screen_height));
}


void MainState::Init() {
  printf("hello world!\n");
  corgi::Entity entity = entity_manager_.AllocateNewEntity();
  entity_manager_.RegisterSystem(&common_system_);
  entity_manager_.RegisterSystem(&test_system_);
  entity_manager_.RegisterSystem(&test_system2_);
  entity_manager_.RegisterSystem(&transform_system_);
  entity_manager_.RegisterSystem(&sprite_system_);
  entity_manager_.RegisterSystem(&physics_system_);
  entity_manager_.RegisterSystem(&fountain_projectiles_);

  entity_manager_.set_max_worker_threads(0);

  entity_manager_.FinalizeSystemList();

  test_system2_.AddEntity(entity);
  //entity_manager_.AddComponent(entity);

  corgi::Entity spritetest;
  SpriteData* sprite_data;
  TransformData* transform_data;

  spritetest = entity_manager_.AllocateNewEntity();
  entity_manager_.AddComponent<SpriteSystem>(spritetest);
  sprite_data = entity_manager_.GetComponentData<SpriteData>(spritetest);
  transform_data = entity_manager_.GetComponentData<TransformData>(spritetest);

  sprite_data->tint = vec4(1.0f, 0.5f, 0.25f, 1.0f);
  transform_data->position = vec3(200.0f, 200.0f, 0.0f);
  sprite_data->size = vec2(100, 50);
  transform_data->scale = vec2(2.0f, 2.0f);
  //------
  spritetest = entity_manager_.AllocateNewEntity();
  entity_manager_.AddComponent<SpriteSystem>(spritetest);
  sprite_data = entity_manager_.GetComponentData<SpriteData>(spritetest);
  transform_data = entity_manager_.GetComponentData<TransformData>(spritetest);

  sprite_data->tint = vec4(0.5f, 1.0f, 0.25f, 1.0f);
  transform_data->position = vec3(300.0f, 50.0f, 0.0f);
  sprite_data->size = vec2(40, 60);
  transform_data->scale = vec2(1.0f, 1.0f);
  transform_data->orientation = quat::FromAngleAxis(2.3f, vec3(0, 0, 1));

	time_direction_ = 1;
}


void MainState::Render(corgi::WorldTime delta_time) {
  glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  sprite_system_.RenderSprites();
  SDL_GL_SwapWindow(common_system_.CommonData()->window);
}


void MainState::Update(corgi::WorldTime delta_time) {
  //printf("---------------------------------------------\n");
  //printf("start of update!\n");
  //printf("---------------------------------------------\n");

	if (time_direction_ == 1) {
		entity_manager_.UpdateSystems(delta_time);
	} else if (time_direction_ == -1) {
		corgi::WorldTime target_time = entity_manager_.CurrentTimestamp() - 1;
		bool problems = entity_manager_.RewindToTimestamp(target_time);
		if (!problems) {
			entity_manager_.AdvanceToTimestamp(target_time);
		}
	}

  SDL_Event event;

	//if (true) {
  if (time_direction_ == 1 && rnd() < 0.2) {
	//if (entity_manager_.CurrentTimestamp() % 60 == 0){
    corgi::Entity particle_thing = entity_manager_.AllocateNewEntity();
    entity_manager_.AddComponent<FountainProjectile>(particle_thing);
    
  }
  /*
  SDL_Log("\nFountain Projectile: %d\nPhysics: %d\nSprite: %d\n",
    fountain_projectiles_.EntityCount(),
    physics_system_.EntityCount(),
    sprite_system_.EntityCount());
    */
	while (SDL_PollEvent(&event)) {
    // We are only worried about SDL_KEYDOWN and SDL_KEYUP events
		switch (event.type) {
		case SDL_KEYDOWN:
			SDL_Log("Key press detected\n");
			break;

		case SDL_KEYUP:
			SDL_Log("Key release detected\n");

			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				EndState();
				break;
			case SDLK_RETURN:
				SDL_Log("Return pressed!");
				//problems = entity_manager_.RewindToTimestamp(entity_manager_.CurrentTimestamp() - 60);
				//SDL_Log(problems ? "Could not rewind." : "Rewound!");

				if (time_direction_ != 0) {
					time_direction_ = 0;
				} else {
					time_direction_ = 1;
				}
				break;
			case SDLK_LEFT:
				time_direction_ = -1;
				break;
			case SDLK_RIGHT:
				time_direction_ = 1;
				break;
			default:
				break;
			}
		



    default:
      break;
    }
  }
}
