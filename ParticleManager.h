#pragma once
#include <vector>
#include "ParticleObj.h"
#include "Shape.h"

/// <summary>
/// The particle manager is the only class the user should have to interact with. It contains all the particles in the system and is in charge of their creation, updates and rendering
/// </summary>
class ParticleManager
{
public:

	/// <summary>
	/// This is a holder for the colour transitions for the particles
	/// </summary>
	struct ColourLerper
	{
		/// <summary>
		/// the colour for this transition
		/// </summary>
		SDL_Color _colour;
		/// <summary>
		/// The duration for this transition
		/// </summary>
		float _durationOfColour;
	};

	/// <summary>
	/// This is the settings that are passed to the constructor for the ParticleManager. The user can initialise any variable they want, or none at all. Any values not initialised will have default values.
	/// </summary>
	class ParticleSettings
	{
	public:
		ParticleSettings()
			:_positionToParentTo(nullptr),
			_offsetFromParent(Vector2(0, 0)),
			_startingVelocity(0),
			_endingVelocity(0),
			_velVariation(0.25f),
			_emissionRate(0.02f),
			_timeToLiveVariation(0),
			_minTTL(-1),
			_maxTTL(-1),
			_particleSize(1.6f),
			_texture(nullptr),
			_shapeType(Shape::ShapeType::Pentagon),
			_coloursToLerp(std::vector<ColourLerper>()),
			_rotationSpeed(0)
		{

		}

		/// <summary>
		/// This is a pointer to a Vector2 that the particleManager will be parented to. This can be a static value, or a changing one; the ParticleManager will follow it.
		/// Defualt Value:  new Vector2(300, 300)
		/// </summary>
		Vector2 * _positionToParentTo;

		/// <summary>
		/// If you want the ParticleManager to be offsetted from the _positionToParentTo, this value will be added onto the position.
		/// Defualt Value:  Vector2(0,0)
		/// </summary>
		Vector2 _offsetFromParent;

		/// <summary>
		/// The starting velocity of particles.
		/// Defualt Value:  0
		/// </summary>
		float _startingVelocity;

		/// <summary>
		/// The ending velocity of particles.
		/// Defualt Value:  0
		/// </summary>
		float _endingVelocity;

		/// <summary>
		/// The variation in velocity for the particles. Increasing this value will increase the angle of the "cone" particles will shoot off in
		/// Defualt Value:  0.25f
		/// </summary>
		float _velVariation;

		/// <summary>
		/// The time between emitting a particle - be careful when edditing this as it will have massive effect on framerate! 
		/// Defualt Value:  0.02f
		/// </summary>
		float _emissionRate;

		/// <summary>
		/// The difference in time to live values for the particles. 
		/// Defualt Value:  0
		/// </summary>
		float _timeToLiveVariation;

		/// <summary>
		/// The minimum time to live for the particles
		/// Defualt Value:  the length of the colour transitions supplied, if no transitions are supplied, it is the length of the default transition, which lasts for 2 seconds
		/// </summary>
		float _minTTL;

		/// <summary>
		/// The maximum time to live for the particles
		/// Defualt Value:  the length of the colour transitions supplied, if no transitions are supplied, it is the length of the default transition, which lasts for 2 seconds
		/// </summary>
		float _maxTTL;

		/// <summary>
		/// The size of the particles.
		/// Defualt Value:  1.6f
		/// </summary>
		float _particleSize;

		/// <summary>
		/// The texture of the particle. (Particles either have a texture or a shape)
		/// Defualt Value:  nullptr
		/// </summary>
		SDL_Texture * _texture;

		/// <summary>
		/// The shape of the particle. (Particles either have a texture or a shape)
		/// Defualt Value:  Pentagon
		/// </summary>
		Shape::ShapeType _shapeType;

		/// <summary>
		/// A vector of ColourLerper's. This contains all the colours your shapes will transition between. Does not apply to textures.
		/// Defualt Value:  white for 2 seconds
		/// </summary>
		std::vector<ColourLerper> _coloursToLerp;

		/// <summary>
		/// The speed your shapes will rotate at.
		/// Defualt Value:  0.0f
		/// </summary>
		float _rotationSpeed;
	};

	/// <summary>
	/// This function will initialise all the preset's values. Mandatory to call this before initialising a particle system to a preset.
	/// </summary>
	static void LoadPresets(SDL_Renderer* pRenderer);

	/// <summary>
	/// The Rocket Thruster preset. This emulates a rocket's fire/smoke trail
	/// </summary>
	static ParticleSettings _ROCKET_THRUSTER_PRESET;

	/// <summary>
	/// The footsteps preset will emulate a person walking around.
	/// </summary>
	static ParticleSettings _FOOTSTEPS_PRESET;

	/// <summary>
	/// This tron preset will emulate the trail of the tron bike
	/// </summary>
	static ParticleSettings _TRON_PRESET;

	/// <summary>
	/// The star preset will make stars of random scale 
	/// </summary>
	static ParticleSettings _STAR_PRESET;



	/// <summary>
	/// This will get a particleObj's colour given how long it's been alive.
	/// </summary>
	/// <param name="pAliveTime">The amount of time this particle has been alive (passed in as ms).</param>
	SDL_Color GetColour(float pAliveTime);


	/// <summary>
	/// The constructor of the ParticleManager
	/// </summary>
	/// <param name="pSettings">The settings of the particleManager.</param>
	/// <param name="pRenderer">The renderer that the ParticleManager will renderer to.</param>
	ParticleManager(ParticleSettings pSettings = ParticleSettings(), SDL_Renderer * pRenderer = nullptr);

	~ParticleManager() { }

	/// <summary>
	/// The update will check if it should remove old ParticleObj's and create new ones. It will move itself and update all the ParticleObj's it contains.
	/// </summary>
	/// <param name="pDT">The delta time in ms</param>
	void update(float pDT);

	void SetUpRocketThruster();
	void GetDefaultColours();
	void SetupTron();
	void SetUpStarPreset();
	void SetUpFootsteps();
	void SetUpTexture();


	//button functions
	std::string IncrementShapeType();
	std::string DecrementShapeType();
	static std::string ChangeEmissionRate(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeMinTimeToLive(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeMaxTimeToLive(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingVelocity(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingVelocity(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeConeAngle(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeParticleType(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeParticleSize(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingRColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingGColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingBColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingAColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingRColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingGColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingBColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingAColour(ParticleManager * pManager, bool pIncrement);

	/// <summary>
	/// This will render all the ParticleObj's to the screen
	/// </summary>
	void render();

	/// <summary>
	/// This will switch the particle system on or off.
	/// </summary>
	void TurnOnOff() { _shouldSystemEmit = !_shouldSystemEmit; }


	/// <summary>
	/// A vector holding all the colour transitions the particles will take
	/// </summary>
	std::vector<ColourLerper> _colourLerpingList;

	/// <summary>
	/// The minimum time to live for particles
	/// </summary>
	float _minTTL;

	/// <summary>
	/// The maximum time to live for particles
	/// </summary>
	float _maxTTL;

	/// <summary>
	/// The size of the particles
	/// </summary>
	float _particleSize;

	/// <summary>
	/// The ending velocity of the particles
	/// </summary>
	Shape::ShapeType _shapeType;

	/// <summary>
	/// This is a list holding the previous velocities, I use this to stop sudden changes to the particle emission direction, instead it's an average of the previous X velocities
	/// </summary>
	std::vector<Vector2> _velocityList;

private:
	

	/// <summary>
	/// This will create a particle, and initialise it's settings
	/// </summary>
	/// <param name="pDir">the direction the particle should be moving in. This by default is the opposite direction of the particle manager</param>
	void SpawnParticle(Vector2 pDir);

	/// <summary>
	/// The variation in velocity that the particles will spawn with. Modifiying this value will change the angle of the "cone" that the particles shoot off in
	/// </summary>
	float _particleVelVariation;

	/// <summary>
	/// The starting velocity of the particles
	/// </summary>
	float _startingVelocity;

	/// <summary>
	/// The ending velocity of the particles
	/// </summary>
	float _endingVelocity;

	/// <summary>
	/// The time in ms between the 
	/// </summary>
	float _emissionRate;
	
	/// <summary>
	/// This holds the texture of the particles, if there is one.
	/// </summary>
	SDL_Texture * _texture;

	/// <summary>
	/// Should the system spawn new particles? This is changed by calling the TurnOnOff() function
	/// </summary>
	bool _shouldSystemEmit;

	/// <summary>
	/// The list of particleObj's
	/// </summary>
	std::vector<ParticleObj *> _particleList;

	/// <summary>
	/// The position that the particle system will be attached to.
	/// </summary>
	Vector2 * _positionToParentTo;

	/// <summary>
	/// The current position of the system
	/// </summary>
	Vector2 _position;

	/// <summary>
	/// The offset the system is located from the positionToParentTo
	/// </summary>
	Vector2 _offset;

	/// <summary>
	/// The current velocity of the system. This is equal to the average of the previous 3 velocities.
	/// </summary>
	Vector2 _currentVelocity;

	/// <summary>
	/// How long in ms since the particle system emitted a particle
	/// </summary>
	float _timeSinceEmit;

	/// <summary>
	/// The address of the renderer we will draw to.
	/// </summary>
	SDL_Renderer * _renderer;

	/// <summary>
	/// The rotational speed of the particles once they are emitted
	/// </summary>
	float _rotationSpeed;
};

