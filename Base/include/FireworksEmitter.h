#ifndef FIREWORKEMITTER_H
#define FIREWORKEMITTER_H

#include "stdafx.h" 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>

#include <Thor/Particles.hpp>
#include <Thor\Particles\ParticleSystem.hpp>
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include <Thor/Math/Distributions.hpp>
#include <array>
#include <memory>

// Various constants as firework parameters
const sf::Time	explosionInterval = sf::seconds(1.f);
const sf::Time	explosionDuration = sf::seconds(0.2f);
const sf::Time	tailDuration = sf::seconds(2.5f);
const sf::Int64	tailsPerExplosion = 15;

// Array with possible colors for explosions
const std::array<sf::Color, 9> particleColors =
{
	sf::Color(100, 255, 135),	// light green
	sf::Color(175, 255, 135),	// lime green
	sf::Color(85, 190, 255),	// light blue
	sf::Color(255, 145, 255),	// pink
	sf::Color(100, 100, 255),	// indigo
	sf::Color(140, 250, 190),	// turquoise
	sf::Color(255, 135, 135),	// red
	sf::Color(240, 255, 135),	// light yellow
	sf::Color(245, 215, 80),	// light orange
};

// Custom emitter that groups particles in tails
class ExplosionEmitter
{
public:
	explicit ExplosionEmitter(sf::Vector2f position)
		: mAccumulatedTime(sf::Time::Zero)
		, mPosition(position)
		, mColor(particleColors[thor::random(0u, particleColors.size() - 1u)])
	{
	}

	void operator() (thor::EmissionInterface& system, sf::Time dt)
	{
		const sf::Time tailInterval = explosionDuration / tailsPerExplosion;

		// Accumulate passed time. If enough time has passed (tailInterval), emit a new tail and decrease accumulator.
		mAccumulatedTime += dt;
		while (mAccumulatedTime - tailInterval > sf::Time::Zero)
		{
			emitTail(system);
			mAccumulatedTime -= tailInterval;
		}
	}

private:
	void emitTail(thor::EmissionInterface& system)
	{
		// Create initial direction with random vector length and angle
		thor::PolarVector2f velocity(thor::random(30.f, 70.f), thor::random(0.f, 360.f));

		// Create particle at position of explosion, with emitter-specific color and at 80% initial scale
		thor::Particle particle(tailDuration);
		particle.position = mPosition;
		particle.color = mColor;
		particle.scale *= 0.8f;

		// A tail contains 25 particles with different speeds and scales.
		// The largest particles move fastest, leading to a comet-like tail effect.
		for (sf::Int64 i = 0; i < 25; ++i)
		{
			// Decrease scale continuously
			particle.scale *= 0.95f;

			// Decrease speed continuously
			velocity.r *= 0.96f;
			particle.velocity = velocity;

			// Add adapted particle to particle system
			system.emitParticle(particle);
		}
	}

private:
	sf::Time		mAccumulatedTime;
	sf::Vector2f	mPosition;
	sf::Color		mColor;
};

// Custom affector that fades particles out and accelerates them according to scale
class ExplosionAffector
{
public:
	void operator() (thor::Particle& particle, sf::Time dt)
	{
		particle.position += particle.velocity * dt.asSeconds();

		// Let particles continuously fade out (particles with smaller scale have already lower alpha value at beginning)
		particle.color.a = static_cast<sf::Uint8>(256 * thor::getRemainingRatio(particle) * particle.scale.x);
	}
};

#endif