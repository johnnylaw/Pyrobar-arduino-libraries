#include <Vector.h>
#include <SimpleTimer.h>
#include "PyrobarConstants.h"
#include "PyrobarSlaveOnlyConstants.h"
#include "ZoneMapping.h"
#include "LightStripInfo.h"
#include "LightStrip.h"
#include "./ZoneInstructions.h"

static ZoneStripMapping zoneStripMappingSets[11][3];

const int stripCount = 8;
const int highestStripLength = 800;
RGBColor tempColors[stripCount][highestStripLength];

PololuLedStrip<53> strip0;
PololuLedStrip<51> strip1;     // If there's a need for another strip, this pin is wired up.
PololuLedStrip<49> strip2;
PololuLedStrip<47> strip3;
PololuLedStrip<45> strip4;
PololuLedStrip<43> strip5;
PololuLedStrip<37> strip6;
PololuLedStrip<35> strip7;

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
  {&strip2, {5, 300}},   // Bar ceiling (1 zone)
  {&strip3, {5, 300}},   // Bar surface (1 zone)
  {&strip4, {12, 19}},        // DJ booth (same zone as bar surface)
  {&strip5, {5, 45}},         // Front pillars
  {&strip6, {12, 93}},         // Steps (1 zone)
  {&strip7, {5, 800}},        // Undercarriage (1 zone)
};

void createZoneMappings(void) {
// ZoneStripMapping is struct with:
//   - (int) strip number
//   - (int) starting bulb of zone in strip
//   - (int) count of bulbs following starting bulb
// Format:
//   zoneStripMappingSets[zone number].push(ZoneStripMapping{strip, starting bulb, bulb count})

  zoneStripMappingSets[0][0] = {0, 0, 16};     // Crane ring
  zoneStripMappingSets[1][0] = {0, 26, 22};    // Crane top (16 - 19 black)
  zoneStripMappingSets[2][0] = {0, 48, 27};    // Crane middle
  zoneStripMappingSets[3][0] = {0, 75, 28};    // Crane bottom
  zoneStripMappingSets[4][0] = {2, 0, 130};    // Bar ceiling (130 - 169 black)
  zoneStripMappingSets[4][1] = {2, 170, 130};  // Bar ceiling
  zoneStripMappingSets[5][0] = {3, 0, 130};    // Bar surface (130 - 169 black)
  zoneStripMappingSets[5][1] = {3, 170, 130};  // Bar surface ...
  zoneStripMappingSets[5][2] = {4, 0, 19};     //   ... and DJ booth
  zoneStripMappingSets[6][0] = {5, 30, 15};    // Pillar high
  zoneStripMappingSets[7][0] = {5, 15, 15};    // Pillar mid
  zoneStripMappingSets[8][0] = {5, 0, 15};     // Pillar low
  zoneStripMappingSets[9][0] = {6, 0, 12};     // Steps
  zoneStripMappingSets[9][1] = {6, 12, 50};     // Steps
  zoneStripMappingSets[9][2] = {6, 50, 15};     // Steps
  zoneStripMappingSets[10][0] = {7, 0, 800};  // Undercarriage
}

const int zoneStripMappingSetSizes[11] = {1, 1, 1, 1, 2, 3, 1, 1, 1, 3, 1};

void writeBuffer(int zoneIndex, uint8_t red, uint8_t green, uint8_t blue) {
  ZoneStripMappingSet mappingSet = zoneStripMappingSets[zoneIndex];
  int mappingSetSize = zoneStripMappingSetSizes[zoneIndex];
  for (int mapIndex = 0; mapIndex < mappingSetSize; mapIndex++) {
    ZoneStripMapping mapping = mappingSet[mapIndex];
    RGBColor color;
    if (zoneIndex == 9 && mapIndex == 1) { // weird strip spliced with other strip
      color = RGBColor(red, green, blue, { 8, 100, 0 });
    } else {
      color = RGBColor(red, green, blue, lightStrips[mapping.strip].info);
    }
    for (int bulb = mapping.start; bulb < mapping.start + mapping.count; bulb++) {
      tempColors[mapping.strip][bulb] = color;
    }
  }
}

void printMapping(ZoneStripMapping mapping) {
  Serial.print("strip: ");
  Serial.print(mapping.strip); Serial.print(", start: ");
  Serial.print(mapping.start); Serial.print(", count: ");
  Serial.println(mapping.count);
}

void writeStrips() {
  for (int stripInd = 0; stripInd < stripCount; stripInd++) {
    LightStrip strip = lightStrips[stripInd];
    strip.pololuStrip->write(tempColors[stripInd], strip.info.count);
  }
}

void resetStrips() {
  for (int zoneIndex = 0; zoneIndex < zoneCount; zoneIndex++) {
    for (int bulbIndex = 0; bulbIndex < highestStripLength; bulbIndex++) {
      tempColors[zoneIndex][bulbIndex] = RGBColor();
    }
  }
  writeStrips();
}

int bufferPtr = 0;
void setUpAndWriteStrips() {
//  Serial.println("In setUpAndWriteStrips(void)");
  bufferPtr = (bufferPtr + 10) % instructionLength;
  for (int zoneIndex = 0; zoneIndex < zoneCount; zoneIndex++) {
    RGBColor color = zoneInstructions[zoneIndex][bufferPtr];
    writeBuffer(zoneIndex, color.red, color.green, color.blue);
  }
  writeStrips();
}

void printZoneAtIndex(int zoneIndex, unsigned char red, unsigned char green, unsigned char blue) {
  Serial.print("Zone "); Serial.print(zoneIndex); Serial.print(": ");
  Serial.print(red); Serial.print(", "); Serial.print(green); Serial.print(", "); Serial.println(blue);
}

void printColor(RGBColor color) {
  Serial.print(color.red); Serial.print(", ");
  Serial.print(color.green); Serial.print(", ");
  Serial.println(color.blue);
}

SimpleTimer timer;

void setup() {
  Serial.begin(115200);
  createZoneMappings();

  Serial.print("Strip count: "); Serial.println(stripCount);
  Serial.print("Size of tempColors: "); Serial.println(sizeof(tempColors));

  // timer.setInterval(100, setUpAndWriteStrips);
}

void loop() {
  setUpAndWriteStrips();
  // delay(10000);
}
