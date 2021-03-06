#include <Wire.h>
#include <Vector.h>
#include "PyrobarConstants.h"
#include "PyrobarSlaveOnlyConstants.h"
#include "ZoneMapping.h"
#include "LightStripInfo.h"
#include "LightStrip.h"

#define SLAVE 0

#if SLAVE == 0

static ZoneStripMappingSet zoneStripMappingSets[9]; // std::vector
const int zoneCount = 9;
const int stripCount = 6;
RGBColor tempColors[stripCount][300]; // Second size must be that of strip with highest number of bulbs

PololuLedStrip<53> strip0;
PololuLedStrip<51> strip1;     // If there's a need for another strip, this pin is wired up.
PololuLedStrip<49> strip2;
PololuLedStrip<47> strip3;
PololuLedStrip<45> strip4;
PololuLedStrip<43> strip5;

// LightStrip is struct with
//   - PololuLedStrip pointer
//   - LightStripInfo, which is struct with
//       - (int) strip voltage
//       - (int) strip bulb count
//       - (int) maximum total brightness of all 3 channels (optional)
//            NOTE: Max brightness is dependent on physical power available
//                  to the strip, as without this, dimming and loss of color
//                  (particularly blue) results in longer strips. Leaving this blank
//                  in struct initialization results in full brightness always.
// None of the strip bulb counts should be higher than secondary size of tempColors array
LightStrip lightStrips[] = {
  {&strip0, {12, 104}},       // Crane (4 zones)
  {&strip1, {12, 0}},
  {&strip2, {5, 300, 400}},   // Bar ceiling (1 zone)
  {&strip3, {5, 300, 400}},   // Bar surface (1 zone)
  {&strip4, {12, 19}},        // DJ booth (same zone as bar surface)
  {&strip5, {5, 45}},         // Front pillars
};

#elif SLAVE == 1

static ZoneStripMappingSet zoneStripMappingSets[2];
const int zoneCount = 2;
const int stripCount = 2;
RGBColor tempColors[stripCount][800];

PololuLedStrip<37> strip0;
PololuLedStrip<35> strip1;

LightStrip lightStrips[] = {
  {&strip0, 12, 93},         // Steps (1 zone)
  {&strip1, 5, 800},        // Undercarriage (1 zone)
};

#endif

void createZoneMappings(void) {
#if SLAVE == 0

// ZoneStripMapping is struct with:
//   - (int) strip number
//   - (int) starting bulb of zone in strip
//   - (int) count of bulbs following starting bulb
// Format:
//   zoneStripMappingSets[zone number].push(ZoneStripMapping{strip, starting bulb, bulb count})
  zoneStripMappingSets[0].push_back(ZoneStripMapping{0, 0, 16});     // Crane ring
  zoneStripMappingSets[1].push_back(ZoneStripMapping{0, 26, 22});    // Crane top (16 - 19 black)
  zoneStripMappingSets[2].push_back(ZoneStripMapping{0, 48, 27});    // Crane middle
  zoneStripMappingSets[3].push_back(ZoneStripMapping{0, 75, 28});    // Crane bottom
  zoneStripMappingSets[4].push_back(ZoneStripMapping{2, 0, 130});    // Bar ceiling (130 - 169 black)
  zoneStripMappingSets[4].push_back(ZoneStripMapping{2, 170, 130});  // Bar ceiling
  zoneStripMappingSets[5].push_back(ZoneStripMapping{3, 0, 130});    // Bar surface (130 - 169 black)
  zoneStripMappingSets[5].push_back(ZoneStripMapping{3, 170, 130});  // Bar surface ...
  zoneStripMappingSets[5].push_back(ZoneStripMapping{4, 0, 19});     //   ... and DJ booth
  zoneStripMappingSets[6].push_back(ZoneStripMapping{5, 30, 15});    // Pillar high
  zoneStripMappingSets[7].push_back(ZoneStripMapping{5, 15, 15});    // Pillar mid
  zoneStripMappingSets[8].push_back(ZoneStripMapping{5, 0, 15});     // Pillar low

#elif SLAVE == 1

  zoneStripMappingSets[0].push_back(ZoneStripMapping{0, 0, 93});     // Steps
  zoneStripMappingSets[1].push_back(ZoneStripMapping{1, 0, 800});  // Undercarriage

#endif

}

void writeBuffer(int zoneIndex, uint8_t red, uint8_t green, uint8_t blue) {
  ZoneStripMappingSet mappingSet = zoneStripMappingSets[zoneIndex];
  for (int mapIndex = 0; mapIndex < mappingSet.size(); mapIndex++) {
    ZoneStripMapping mapping = mappingSet[mapIndex];
    RGBColor color = RGBColor(red, green, blue, lightStrips[mapping.strip].info);
    for (int address = mapping.start; address < mapping.start + mapping.count; address++) {
      tempColors[mapping.strip][address] = color;
    }
  }
}

void writeStrips() {
  for (int stripInd = 0; stripInd < stripCount; stripInd++) {
    LightStrip strip = lightStrips[stripInd];
    strip.pololuStrip->write(tempColors[stripInd], strip.info.count);
  }
}

void parseLightValues(int packetSize) {
  Serial.println("Hello");
  for (int zoneIndex = 0; zoneIndex < zoneCount; zoneIndex++) {
    uint8_t r = Wire.read();
    uint8_t g = Wire.read();
    uint8_t b = Wire.read();
    writeBuffer(zoneIndex, r, g, b);
    packetSize -= 3;
    // Serial.print(zoneIndex); Serial.print(": "); Serial.print(r); Serial.print(", "); Serial.print(g); Serial.print(", "); Serial.println(b); 
  }
  while (packetSize--) { Wire.read(); }
  writeStrips();
}

void setup() {
  createZoneMappings();
  Serial.begin(115200);

  Serial.print("Setting up I2C at "); Serial.println(BASE_I2C_ADDRESS + SLAVE);
  Wire.begin(BASE_I2C_ADDRESS + SLAVE);
  Wire.onReceive(parseLightValues);

  Serial.print("Slave board "); Serial.println(SLAVE);
  Serial.print("Zone count: "); Serial.println(zoneCount);
  Serial.print("Strip count: "); Serial.println(stripCount);
  Serial.print("Size of tempColors: "); Serial.println(sizeof(tempColors));

  delay(1000);
}

void loop() {
  delay(1000);
}
