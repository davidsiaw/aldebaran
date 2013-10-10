#ifndef IMAGESCENE_HPP
#define IMAGESCENE_HPP

#include "scene_interface.hpp"

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>

// Scene that displays a dialog
class ImageScene : public SceneInterface
{
	std::string filename;
	std::tr1::shared_ptr<SDL_Texture> image;

	bool running;

public:
	ImageScene(std::string filename);
	virtual ~ImageScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;

	void SetRunning(bool yesno);

	// TODO
	// image position/width/height

	// menu

};

#endif // IMAGESCENE_HPP
