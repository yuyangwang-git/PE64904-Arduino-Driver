// PE64904 SPI Control Program
#include <SPI.h>
#include <ctype.h>

// Define SPI pins
#define PIN_SEN 10 // Chip select (SEN)
#define PIN_SCL 13 // SPI Clock (SCL)
#define PIN_SDA 11 // SPI Data (SDA)

// Auto-step mode flag
bool autoStepMode = false;

// Current state
uint8_t currentState = 0;

void setup()
{
  initializeSerial();
  initializeSPI();
  resetDevice();
  printSystemInfo();
}

void loop()
{
  if (autoStepMode)
  {
    static unsigned long lastStepTime = 0;
    if (millis() - lastStepTime > 1000)
    { // Auto step every second
      lastStepTime = millis();
      setCapacitanceState((currentState + 1) % 32); // Cycle through states
    }
  }
  handleSerialInput();
}

// Initialize serial communication
void initializeSerial()
{
  Serial.begin(9600);
  while (!Serial)
    ; // Wait for the Serial port to be ready
}

// Initialize SPI communication
void initializeSPI()
{
  pinMode(PIN_SEN, OUTPUT);
  digitalWrite(PIN_SEN, HIGH); // Disable chip initially
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
}

// Reset device to initial state
void resetDevice()
{
  setCapacitanceState(0);
}

// Handle incoming serial commands
void handleSerialInput()
{
  static char inputBuffer[10];
  static uint8_t inputIndex = 0;

  while (Serial.available() > 0)
  {
    char receivedChar = Serial.read();
    if (receivedChar == '\n' || receivedChar == '\r')
    {
      inputBuffer[inputIndex] = '\0'; // Null-terminate the string
      processCommand(inputBuffer);
      inputIndex = 0;
    }
    else if (inputIndex < sizeof(inputBuffer) - 1)
    {
      inputBuffer[inputIndex++] = receivedChar;
    }
  }
}

// Process received commands
void processCommand(const char *input)
{
  if (strncmp(input, "SET ", 4) == 0)
  {
    int newState = atoi(input + 4);
    if (newState >= 0 && newState <= 31)
    {
      setCapacitanceState(newState);
    }
    else
    {
      Serial.println("Error: State must be between 0 and 31.");
    }
  }
  else if (strcmp(input, "GET") == 0)
  {
    feedbackState();
  }
  else if (strcmp(input, "RESET") == 0)
  {
    resetDevice();
  }
  else if (strcmp(input, "START") == 0)
  {
    autoStepMode = true;
    Serial.println("Auto step mode started.");
  }
  else if (strcmp(input, "STOP") == 0)
  {
    autoStepMode = false;
    Serial.println("Auto step mode stopped.");
  }
  else
  {
    Serial.println("Error: Unknown command.");
  }
}

// Set capacitance state
void setCapacitanceState(uint8_t state)
{
  digitalWrite(PIN_SEN, LOW);
  SPI.transfer(state);
  digitalWrite(PIN_SEN, HIGH);
  currentState = state;
  feedbackState();
}

// Feedback current state
void feedbackState()
{
  float capacitance = calculateCapacitance(currentState);
  float resistance = calculateResistance(currentState);
  Serial.println("=== PE64904 State Update ===");
  Serial.print("New State: ");
  Serial.println(currentState);
  Serial.print("Capacitance: ");
  Serial.print(capacitance, 2);
  Serial.println(" pF");
  Serial.print("Resistance: ");
  Serial.print(resistance, 2);
  Serial.println(" Ω");
  Serial.println("============================");
}

// Calculate capacitance based on state
float calculateCapacitance(uint8_t state)
{
  return 0.129 * state + 0.6;
}

// Calculate resistance based on state
float calculateResistance(uint8_t state)
{
  return 20.0 / (state + 20.0 / (state + 0.7)) + 0.7;
}

// Print system information
void printSystemInfo()
{
  Serial.println("=== PE64904 System Information ===");
  Serial.println("SPI Settings:");
  Serial.println("  Clock Divider: 8");
  Serial.println("  Data Mode: SPI_MODE0");
  Serial.println("  Bit Order: MSBFIRST");
  Serial.println("Use commands: SET <state>, GET, RESET, START, STOP.");
  Serial.println("====================================");
}
