#ifndef INPUTSTATE_HPP
#define INPUTSTATE_HPP

class InputState
{	
	char down[32];
	char pressed[32];
	char released[32];

	bool fingered;
	float fingerx;
	float fingery;
    
    float accelx;
    float accely;
    float accelz;

public:

	enum Key
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		A,
		B,
		X,
		Y,
		L1,
		L2,
		R1,
		R2,
		START,
	};

	InputState();

	bool GetButtonState(Key key) const;
	bool GetButtonPressed(Key key) const;
	bool GetButtonReleased(Key key) const;
	void SetButtonState(Key key, bool state);
	void SetButtonPressed(Key key, bool state);
	void SetButtonReleased(Key key, bool state);

	bool GetFingered() const;
	float GetFingerX() const;
	float GetFingerY() const;

	void SetFingered(bool state);
	void SetFingerX(float fx);
	void SetFingerY(float fy);
    
    float GetAccelX() const;
    float GetAccelY() const;
    float GetAccelZ() const;
    
    void SetAccelX(float amount);
    void SetAccelY(float amount);
    void SetAccelZ(float amount);

};

#endif // INPUTSTATE_HPP
