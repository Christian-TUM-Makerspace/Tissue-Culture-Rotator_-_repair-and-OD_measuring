//Test program for saving data as a String() object and sending it via Serial.print() for every measurement revolution

//Form of the string:
//data as keyword > comma and space > number of the curren measurement revolution > comma and space > 
  // > "measured" value > comma and space 
  //  the line above repeated until tubecount is reached (also ", " in the end)
//example:
  //data, 13, 832, 766, 764, 759, 761, 763, 747, 740, 767, 787, 827, 837, 819, 763, 783, 832, 807, 799, 750, 765, 740, 828, 833, 831, 824, 789, 807, 796, 835, 783, 751, 824, 787, 782, 783, 749, 773, 794, 808, 742, 


//Tubes can be identified through the position in the string

String dataString = "data, ";
int measRevNr = 0;
int randomDecreaser = 1000;
int stepsize = 20;
int tubecount = 40;
int optValTemp =0;
int range=100;

void setup() {
  Serial.begin(115200);
}
void loop() {
  // reset String for every measurement revolution
  dataString = "data, ";
  // Number of the current measurement revolution
  dataString = dataString + measRevNr + ", ";

  //write vaulues for all tubes into string
  for (short i=0; i < tubecount; i++){
    
    optValTemp = random(randomDecreaser,randomDecreaser+range);
    dataString = dataString + optValTemp + ", ";
  }
  Serial.println(dataString);

  // new measurement revolution
  measRevNr += 1;
  //let the average values decrease with every round (remains > 0)
  if (randomDecreaser > 2*stepsize){
      randomDecreaser -= stepsize;
  }
  delay(1000);
}
