


#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
}
#endif

class Profiler
{
public:
  Profiler()
  {
    cyCount = 0;
    cycle = 0, lastCycle = 0;;//ESP.getCycleCount();
    cycleSum = 0;//cycle;
    cycleDiff = 0;
    averageTick = 0;
    averageTime = 0; // micros
    lastIterationCount = 0;
    isValidProfile = 0;
  }

  ~Profiler(){}

  void Reset()
  {
    Profiler();
  }

  void Start()
  {
    isValidProfile = 1;
  }

  void Pause()
  {
    isValidProfile = 0;
  }

  uint16_t Update()
  {

    isValidProfile = isValidProfile;
    boolean whether = (boolean)(1 == isValidProfile);

    if(whether)
    //switch (isValidProfile)
    //{
    //  case 0: break;
    //  case 1:
      {
        //  return;

        lastCycle = cycle;
        cycle = ESP.getCycleCount();
        //cycleSum += cycle;
        cyCount++;

        cycleDiff = (cycle - lastCycle);

        cycleSum += cycleDiff;
      }//break;
      //default: break;
    //}

    return cyCount;

    //Serial.printf("Profiler Cy: %d, %d, %d, %d \n", cycleDiff, cycleSum, cyCount, (cycleSum/cyCount));

  }

  // Can be called at every second
  // Or just before printing
  void CalculateAverageTicks()
  {
    averageTick = (cycleSum/cyCount);
    averageTime = (float)averageTick / 80; // 1 sec => 80MHz; 1micros => 80Hz

    lastIterationCount = cyCount;

    cyCount = 0 ;
    cycleSum = 0;
  }

  float getAverageMicros()
  {
    return averageTime;
  }

  uint32_t getAverageTicks()
  {
    return averageTick;
  }

  uint16_t getLastIterationCount()
  {
    return lastIterationCount;
  }


private:
  uint16_t cyCount;
  uint32_t cycle, lastCycle;//ESP.getCycleCount();
  uint32_t cycleSum;//cycle;
  uint32_t cycleDiff;
  uint32_t averageTick;
  float averageTime; // micros
  uint16_t lastIterationCount;
  uint16_t isValidProfile;
};

Profiler profiler;

void setup() {
  profiler.Start();
  profiler.Update();
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  profiler.Update();
  profiler.Pause();

  profiler.CalculateAverageTicks();
  Serial.printf("LOOPPROF: %d, %d | ", profiler.getLastIterationCount(), profiler.getAverageTicks());// (cycleSum/cyCount));
  Serial.print(profiler.getAverageMicros());
  Serial.printf(" uS | ");

  profiler.Reset();
}

uint32_t ts = 0;
void loop() {
  profiler.Start();
  profiler.Update();

  if(system_get_time() - ts >= 1000000)
  {
    ts = system_get_time();
    profiler.CalculateAverageTicks();
    Serial.printf("LOOPPROF: %d, %d | ", profiler.getLastIterationCount(), profiler.getAverageTicks());// (cycleSum/cyCount));
    Serial.print(profiler.getAverageMicros());
    Serial.printf(" uS\n");
  }

  profiler.Pause();
}
