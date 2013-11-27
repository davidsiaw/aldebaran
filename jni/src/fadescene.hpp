#ifndef FADESCENE_HPP
#define FADESCENE_HPP

#include "scene_interface.hpp"

#include <boost/tr1/memory.hpp>

// Scene that fades in a child scene and then fades out when the scene ends
class FadeScene : public SceneInterface
{
public:
	
	enum Mode
	{
		FadeIn,
		FadeOut
	};


	FadeScene(SceneInterface* scene, Mode mode, double speed = 10);
	virtual ~FadeScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;

	void SetSpeed(double speed);
	double GetSpeed() const;

private:
	SceneInterface* scene;
	std::tr1::shared_ptr<SDL_Texture> renderTarget;
	int w,h;
	Mode mode;
	double opacity;
	double speed;
};

#endif // FADESCENE_HPP
