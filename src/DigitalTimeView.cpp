#include <DigitalTimeView.hpp>
#include <Program.hpp>


Speaker *DigitalTimeView::clock_speaker = new Speaker(100.0f, false);

void DigitalTimeView::cleanup(){
	delete clock_speaker;
}


void set_origin_center(sf::CircleShape& circle){
	// sets the origin of the shape to its local center
	sf::Vector2f origin(0,0);
	for (int i = 0; i < circle.getPointCount()-1; ++i){
		origin += circle.getPoint(i);
	}
	circle.setOrigin(origin/(float)circle.getPointCount());
}



DigitalTimeView::DigitalTimeView(jb::Transform _tf)
	: Entity(_tf),
	  line(Line(_tf, "12:00", 50, 0, 30, JB_WHITE)),
	  top_arrow(8, 3),
	  bottom_arrow(8, 3),
	  index(0),
	  meridiem(POST),
	  lerp(0),
	  lerp_target(5)
{
	// position the selector arrows
	float x_pos = (line.line[index + (2*index)].getPosition().x + line.line[index + (2*index) + 1].getPosition().x) / 2;
	top_arrow.setPosition(x_pos, tf.y - 35);
	top_arrow.setFillColor(JB_GREEN);
	set_origin_center(top_arrow);

	bottom_arrow.setPosition(x_pos, tf.y + 31);
	bottom_arrow.setFillColor(JB_GREEN);
	bottom_arrow.setRotation(180);
	set_origin_center(bottom_arrow);
}


void DigitalTimeView::alter_digit(int direction){
	// NOTE: plan is to just be able to alter the hour number and minute number as a whole.
	// 		This would be easier than having to navigate through each individual digit.

	std::string time_strs = line.get_string();

	if (index == 0){ // modifying hour
		int new_hour = std::stoi(time_strs.substr(0, 2)) + direction;
		std::string new_hour_str = (new_hour >= 10) ? 
			std::to_string(new_hour) : "0" + std::to_string(new_hour);
		if (!jb::clamp(new_hour, 1, 13)){
			line.line[index].setString(new_hour_str[0]);
			line.line[index+1].setString(new_hour_str[1]);
			
		} else { // play error sound
			clock_speaker->play("error.wav");
		}

	} else if (index == 1){ // modifying minute
		int new_minute = std::stoi(time_strs.substr(3, 2)) + direction;
		std::string new_minute_str = (new_minute >= 10) ? 
			std::to_string(new_minute) : "0" + std::to_string(new_minute);
		if (!jb::clamp(new_minute, 0, 60)){
			// + 2 and + 3 to properly map index 1 to values in the line
			line.line[index+2].setString(new_minute_str[0]);
			line.line[index+3].setString(new_minute_str[1]);
			
		} else { // play error sound
			clock_speaker->play("error.wav");
		}
	}
}


void DigitalTimeView::move_selector(int direction){
	index += direction;
	if (!jb::clamp(index, 0, 2)){
		// move selection arrows
		float x_pos = (line.line[index + (2*index)].getPosition().x + line.line[index + (2*index) + 1].getPosition().x) / 2;
		top_arrow.setPosition(x_pos, top_arrow.getPosition().y);
		bottom_arrow.setPosition(x_pos, bottom_arrow.getPosition().y);
	}

}


void DigitalTimeView::switch_meridiem(){
	meridiem = (Meridiem)((meridiem+1) % 2);
}


jb::Time DigitalTimeView::get_time(){
	// returns military time
	jb::Time time(line.get_string());
	if (meridiem == POST){
		time = (time.hour != 12) ? time + 12 * 60 : time; // add 12 hours for PM, unless it is 12 PM
	} else { // meridiem is ANTE
		time = (time.hour != 12) ? time : time + 12 * 60; // keep AM the same, unless it is 12 AM
	}
	return time;
}


void DigitalTimeView::engage(bool value){
	engaged = value;
}


void DigitalTimeView::update(float dt){
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
	if (engaged){
		target.draw(top_arrow);
		target.draw(bottom_arrow);
	}
}



bool DigitalTimeView::handler(sf::Event& event, Program& p){
	if (event.type == sf::Event::KeyPressed){
		switch (event.key.code){

		case sf::Keyboard::K:
			if (LSHIFT_IS_DOWN){
				alter_digit(5);
			} else {
				alter_digit(1);
			}
			return true;

		case sf::Keyboard::J:
			if (LSHIFT_IS_DOWN){
				alter_digit(-5);
			} else {
				alter_digit(-1);
			}
			return true;

		case sf::Keyboard::L:
			move_selector(1);
			return true;

		case sf::Keyboard::H:
			move_selector(-1);
			return true;

		case sf::Keyboard::M:
			switch_meridiem();
			return true;

		case sf::Keyboard::Tab:
			p.engage_with(p.main_tbox);
			return true;
		}
	}
	return false;
}



