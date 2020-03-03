# ShoggothInjector
An HPL2 code injector, for all your fancy Amnesia modding needs.

## Instructions
*This project is in ALPHA. It is not intended to be used in actual custom stories in its current state- as a consequence, the following instructions are for how to run the test file to ensure that the Injector works on your machine.*

### Preparing your Custom Story
1) Into your map *.hps* file, copy the contents of *templates/template.hps*
2) Insert your own HLP script into the section labeled "YOUR MAP CODE."
3) Create injectable variables (currently initialized to 0) using:
* SetGlobalVarInt - *Global Integers;* ***Maximum name length: 15 characters***
* SetLocalVarInt - *Local Integers;* ***Maximum name length: 15 characters***
* SetGlobalSIFloat - *Global Injectable Floats;* ***Maximum name length: 12 characters***
* SetLocalSIFloat - *Local Injectable Floats;* ***Maximum name length: 12 characters***
4) Pass any of these variable names into the Injector, using the instructions under ***Running the Injector***

### Running the Injector
1) Open an instance of Amnesia: The Dark Descent and load your custom story.
2) Run the Injector starting in *Shoggoth Injector.cpp*
3) Follow the console prompts to select your Amnesia instance and the HPL Global/Local variable you wish to load.
4) Run the existing test or write your own by modifying the rest of *Shoggoth Injector.cpp*
