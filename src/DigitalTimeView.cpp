#include <DigitalTimeView.hpp>


sf::Font DigitalTimeView::font;



DigitalTimeView::DigitalTimeView(jb::Transform _tf)
	: tf(_tf),
	  line(Line("12:30", 50, _tf, 0, 30, sf::Color::White)),
	  lerp(0),
	  lerp_target(5)
{
   DigitalTimeView::font.loadFromFile("res/fonts/incon.ttf");

}

DigitalTimeView& DigitalTimeView::operator =(const DigitalTimeView& from){
	if (&from != this){
		tf = from.tf;
		lerp = from.lerp;
		lerp_target = from.lerp_target;
		line = from.line;
	}
	return *this;
}


void DigitalTimeView::update(float dt){
	return;
	float rate = 0.1;
	int target = 255;
	float inc = rate * dt;
	lerp += inc;
	if (lerp >= lerp_target) lerp = 0;
	float clr = cos(180/PI * lerp) + 1;
	
	
	sf::Color last = line.line[2].getFillColor();
	last.a = target/2 * clr;
	line.line[2].setFillColor(last);

}


void DigitalTimeView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(line);
}
