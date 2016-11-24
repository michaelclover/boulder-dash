#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Timer
{
	public:
	    // Default constructor to initialise member variables
		Timer();

		// Start the timer ticking
		void Start();

		// Stop the timer ticking
		void Stop();

		// Pause the timer ticking
		void Pause();

		// Unpause the timer
		void Unpause();

		// Return the number of ticks that have passed since timer start
		Uint32 GetTicks();

		// Returns true if the timer is started
		bool IsStarted();

		// Returns true if the timer is paused
		bool IsPaused();

	private:
	    // The number of ticks that have passed since the timer started
		Uint32 m_startTicks;

		// The number of ticks that have passed whilst the timer has been paused
		Uint32 m_pausedTicks;

		// Is the timer paused?
		bool m_paused;

		// Is the timer started?
		bool m_started;
};

#endif // TIMER_H
