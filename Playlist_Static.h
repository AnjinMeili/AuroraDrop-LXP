#ifndef Playlist_Static_H
#define Playlist_Static_H

// these are work in progress
//
#include "PatternsStatic/PatternStatic_A_Worms.h"
#include "PatternsStatic/PatternStatic_M_SpiralLines.h"
#include "PatternsStatic/PatternStatic_M_Flock.h"
#include "PatternsStatic/PatternStatic_M_FlowField.h"
#include "PatternsStatic/PatternStatic_M_Attract.h"
#include "PatternsStatic/PatternStatic_M_Bounce.h"
#include "PatternsStatic/PatternStatic_X_Atom.h"
#include "PatternsStatic/PatternStatic_X_SimpleStars.h"
#include "PatternsStatic/PatternStatic_X_Swirl.h"

// theses are all just proof of concept from aurora demo
//
#include "PatternsOther/PatternTest.h"
#include "PatternsOther/PatternXIncrementalDrift.h"
#include "PatternsOther/PatternXSpiro.h"
#include "PatternsOther/PatternXSpin.h"
#include "PatternsOther/PatternXRadar.h"
#include "PatternsOther/PatternXWave.h"
#include "PatternsStatic/PatternStatic_X_SpiralingCurves.h"
// #include "PatternsStatic/PatternStatic_OLD_LianLiSL120.h"
#include "PatternsEffects/PatternEffect_XX_NOOP.h"

class Platlist_Static : public Playlist {
  private:

    PatternStaticWorms staticWorms;
    PatternStaticSimpleStars staticSimpleStars;
    PatternStaticAtom staticAtom;
    PatternTest patternTest;
    PatternSpiralLines spiralLines;
    PatternIncrementalDrift incrementalDrift;
    PatternFlock flock;
    PatternFlowField flowField;
    PatternSpiro spiro;
    PatternAttract attract;
    PatternSpin spin;
    PatternStaticBounce staticBounce;
    PatternRadar radar;
    PatternWave wave; 
    PatternStaticSpiralingCurves spiralingCurves;
    PatternStaticSwirl staticSwirl;
    PatternEffectNOOP effectNOOP;

    int currentIndex = 0;
    Drawable* currentItem;

    int getCurrentIndex() {
      return currentIndex;
    }

    const static int PATTERN_COUNT = 8;

    Drawable* shuffledItems[PATTERN_COUNT];

    Drawable* items[PATTERN_COUNT] = {

        &effectNOOP,
        &staticWorms,
        &staticSimpleStars,
        &flock,
        &attract,
        &staticBounce,
        &spiralingCurves,
        &staticSwirl,

        // &flowField,            // works, just not interesting
        // &incrementalDrift,     // works, just not interesting
        // &spiralLines,          // works, just not interesting
        // &radar,                // works, just not interesting
        // &wave,                 // works, just not interesting
        // &spiro,                // works, just not interesting
        // &staticLianLi,         // works, just not interesting
        // &staticAtom,           // works, just not interesting
        // &spin,                 // BAD freezes randomly (LXP confirmed bad)

    };

  public:

    Platlist_Static() {

        // add the items to the shuffledItems array

        for (int a = 0; a < PATTERN_COUNT; a++) {
        
            shuffledItems[a] = items[a];
        
        }

        shuffleItems();

        this->currentItem = items[0];

        for (int i=0; i < CountPlaylistsStatic; i++) {

            this->currentItem->start(i); 

        }

    }

    //char* Drawable::name = (char *)"Patterns";

    // Auroradrop: 
    // Auroradrop: 
    char* getItemName(int _id) {
      return items[_id]->name;      
    }

    int getPatternCount() {
      return PATTERN_COUNT;
    }

    bool getItemEnabled(int _id) {
      return items[_id]->isEnabled();      
    }

    void setItemEnabled(int _id, int value) {

        if (getItemEnabled(_id) != value) {

            Serial.printf("Pattern '%s' changed from %d to %d\n",getItemName(_id),getItemEnabled(_id),value);

        }

        items[_id]->setEnabled((bool)value);

    }

    bool getCurrentItemEnabled() {
      return this->currentItem->isEnabled();      
    }

    // Auroradrop: 
    // Auroradrop: 

    void stop() {
      if (currentItem)
        currentItem->stop();
    }

    void start(uint8_t _pattern) {
      if (currentItem)
        currentItem->start(_pattern);
    }

    void move(int step, uint8_t _pattern) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;

      if (effects.paletteIndex == effects.RandomPaletteIndex)
        effects.RandomPalette();

      moveTo(currentIndex, _pattern);

      //if (!isTimeAvailable && currentItem == &analogClock)
     //   move(step);
    }

    void moveRandom(int step, uint8_t _pattern) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;

      if (effects.paletteIndex == effects.RandomPaletteIndex)
        effects.RandomPalette();

      if (currentItem)
        currentItem->stop();

      currentItem = shuffledItems[currentIndex];

      if (currentItem)
        currentItem->start(_pattern);

     // if (!isTimeAvailable && currentItem == &analogClock)
     //   moveRandom(step);
    }

    void shuffleItems() {

      //return;  // TEMP

      for (int a = 0; a < PATTERN_COUNT; a++)
      {
        int r = random(a, PATTERN_COUNT);
        Drawable* temp = shuffledItems[a];
        shuffledItems[a] = shuffledItems[r];
        shuffledItems[r] = temp;
      }
    }


    unsigned int drawFrame(uint8_t _pattern, uint8_t _total) {
      return currentItem->drawFrame(_pattern, _total);
    }

    void listPatterns() {
      Serial.println(F("{"));
      Serial.print(F("  \"count\": "));
      Serial.print(PATTERN_COUNT);
      Serial.println(",");
      Serial.println(F("  \"results\": ["));

      for (int i = 0; i < PATTERN_COUNT; i++) {
        Serial.print(F("    \""));
        Serial.print(i, DEC);
        Serial.print(F(": "));
        Serial.print(items[i]->name);
        if (i == PATTERN_COUNT - 1)
          Serial.println(F("\""));
        else
          Serial.println(F("\","));
      }

      Serial.println("  ]");
      Serial.println("}");
    }

    char * getCurrentPatternName()
    {
      return currentItem->name;      
    }

    char * getCurrentPatternId()
    {
      return currentItem->id;      
    }


    void moveTo(int index, uint8_t _pattern) {
      if (currentItem)
        currentItem->stop();

      currentIndex = index;

      currentItem = items[currentIndex];

      if (currentItem)
        currentItem->start(_pattern);
    }    

    bool setPattern(String name, uint8_t _pattern) {
      for (int i = 0; i < PATTERN_COUNT; i++) {
        if (name.compareTo(items[i]->name) == 0) {
          moveTo(i, _pattern);
          return true;
        }
      }
      return false;
    }

    bool setPattern(int index, uint8_t _pattern) {
      if (index >= PATTERN_COUNT || index < 0)
        return false;

      moveTo(index, _pattern);

      return true;
    }
};

#endif
