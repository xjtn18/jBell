#include <Sound.hpp>
using namespace aud;



// statics
auto Sound::press_sound 	= UPS("res/sounds/click.wav");
auto Sound::err_sound 		= UPS("res/sounds/error.wav");
auto Sound::create_sound 	= UPS("res/sounds/create.wav");
auto Sound::move_sound 		= UPS("res/sounds/move.wav");
auto Sound::remove_sound 	= UPS("res/sounds/remove.wav");



Sound::Sound(const char* filename, float vol, bool loop){
	buffer.loadFromFile(filename);
	sound.setBuffer(buffer);
	sound.setVolume(vol);
	sound.setLoop(loop);
}

Sound::Sound(float vol, bool loop){
	sound.setVolume(vol);
	sound.setLoop(loop);
}

Sound::Sound(const Sound& from)
	: buffer(from.buffer)
{
	dlog("copy construction");
}

Sound& Sound::operator=(const Sound& from){
	if (this != &from){
		dlog("copy assignment");
		buffer = from.buffer; // creates a COPY of the sound buffer
		sound.setBuffer(buffer); // do not use from.buffer, use our copy
		sound.setVolume(from.sound.getVolume());
		sound.setLoop(from.sound.getLoop());
	}
	return *this;
}

Sound::~Sound(){
}

void Sound::set_sound(std::string sound_name){
	buffer.loadFromFile(sound_name);
	sound.setBuffer(buffer);
}

void Sound::play(){
	sound.play();
}

void Sound::stop(){
	sound.stop();
}

bool Sound::is_playing() const {
	return sound.getStatus() == sf::SoundSource::Playing;
}

void Sound::cleanup(){
	// free all sounds
	Sound::press_sound.reset();
	Sound::err_sound.reset();
	Sound::create_sound.reset();
	Sound::move_sound.reset();
	Sound::remove_sound.reset();
}



