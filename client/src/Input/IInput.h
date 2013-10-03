#pragma once

/**
 *  File header
 */

#include <memory>
#include <vector>
#include <glm/glm.hpp>

/**
 *  Interface to provide platform independent access to platform dependent input events.
 */
class IInput
{
public:
	struct Event;	// Forward declaration, see below

protected:
	std::vector<Event> events;

public:
	typedef std::shared_ptr<IInput> ptr;

	/**
	 *  Enum mapping to physical key on an US keyboard layout.
	 */
	enum class KeyCode	//105 keyboard buttons +1(unknown)
	{
		/// Unknown key. Usually an error if returned.
		UNKNOWN = -1,																							//1
		BACKSPACE = 0x08, TAB, RETURN = 0x0D, BREAK = 0x13, CAPS_LOCK, ESCAPE = 0x1B, SPACE = 0x20,				//7
		PAGE_UP, PAGE_DOWN, END, HOME, LEFT_ARROW, UP_ARROW, RIGHT_ARROW, DOWN_ARROW,							//8
		SINGLE_QUOTE = 0x27, COMMA = 0x2C, DASH = 0x2D, PERIOD = 0x2E, SLASH = 0x2F,							//5
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,																	//10
		SEMICOLON = 0x3B, EQUAL = 0x3D,																			//2
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,					//26
		LEFT_SQUARE_BRACKET, BACK_SLASH, RIGHT_SQUARE_BRACKET, BACK_QUOTE, DEL = 0x7F,							//5
		LEFT_SHIFT, RIGHT_SHIFT, LEFT_CTRL, RIGHT_CTRL, LEFT_ALT, RIGHT_ALT, INSERT, LEFT_SUPER, RIGHT_SUPER,	//9
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,														//12
		SYS_RQ, SCROLL_LOCK, NUM_LOCK,																			//3
		NP_DIVIDE, NP_MULTIPLY, NP_MINUS, NP_PLUS, NP_RETURN, NP_PERIOD,										//6
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,														//10
	};

	/**
	 *  Event for when the mouse cursor has been moved.
	 */
	struct MouseMoveEvent
	{
		/**
		 *  Horizontal position, left to right, in pixels,
		 *  starting with 0 at the left inner border of the window.
		 */
		int posX;
		/**
		 *  Vertical position, top to bottom, in pixels,
		 *  starting with 0 at the top inner border of the window.
		 */
		int posY;
	};

	/**
	 *  Event for when a mouse button has been pressed or released.
	 */
	struct MouseButtonEvent
	{
		/**
		 *  The different supported mouse buttons.
		 */
		enum class Button
		{
			/// Unknown button. Usually an error if returned.
			UNKNOWN,
			LEFT,
			RIGHT,
			/// Middle mouse button, usually from pressing the mouse wheel.
			MIDDLE,
		};

		/**
		 *  The pressed or released button.
		 */
		Button button;
		/**
		 *  The new state of the button, true if pressed, false if released.
		 */
		bool pressed;
	};

	/**
	 *  Event for when a key on the keyboard has been pressed or released.
	 */
	struct KeyEvent
	{
		/**
		 *  The pressed or released key.
		 */
		KeyCode key;
		/**
		 *  The new state of the key, true if pressed, false if released.
		 */
		bool pressed;
	};

	/**
	 * General input event type.
	 */
	struct Event
	{
		/**
		 *  The different possible event types.
		 */
		enum class Type
		{
			MOUSE_MOVE,
			MOUSE_BUTTON,
			KEY,
		};

		union
		{
			KeyEvent keyEvent;
			MouseMoveEvent mouseMoveEvent;
			MouseButtonEvent mouseButtonEvent;
		};

		/**
		 *  The type of this event. Accessing a different event type is invalid.
		 */
		Type type;
	};

	/**
	 *  Get the events since the last time the events was cleared.
	 *
	 *  @param clearEvents if the events will be cleared after they have been returned or if they should be kept
	 *
	 *  @return a list of not previously cleared events
	 */
	std::vector<Event> getEvents(bool clearEvents);
};
