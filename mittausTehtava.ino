/*
Vektorialgebra, mittaustehtävä
Ryhmä 3
*/

//#define DEBUG // Näytä debuggaus tulosteet sarjamonitorilla poistamalla tämän rivin alussa oleva kommentointi

#define MITATTAVIEN_ARVOJEN_LKM 6
#define SENSORI_PINNI A0
#define YOVALON_PINNI 2

// Ylimääräinen funktio, joka sytyttää led-valon (Yövalo) jos 6 sensoriarvon keskiarvo on riittävän alhainen, eli on pimeää :)
void yovalo(int keskiarvo){
  if(keskiarvo < 700){
    digitalWrite(YOVALON_PINNI, HIGH);
    }
    else {
      digitalWrite(YOVALON_PINNI, LOW);
      }
  }

int i = 0; // Indeksin lukumäärä
int tmp = 0; // Muuttuja, johon sensoriarvo tallennetaan väliaikaisesti järjestettäessä mitattuja arvoja nousevaan järjestykseen
int sensoriArvo = 0;
int mitatutSensoriArvot[MITATTAVIEN_ARVOJEN_LKM]; // Taulukko johon 6 mitattua sensoriarvoa talletetaan
float sensoriarvojenSumma = 0; // Flotari muuttuja, jotta voidaan tulostaa keskiarvo kahden desimaalin tarkkuudell.
int suurinMitattuArvo = 0;
int pieninMitattuArvo = 1023; // Suurin mahdollinen sensoriarvo 5V jännitteellä, joten pienin mitattu arvo on varmasti tätä pienempi ja lähtee päivittymään joka mittauksella. Unon A/D-muunnin on 10-bit.

void setup() {
    Serial.begin(9600);
    pinMode(YOVALON_PINNI, OUTPUT);
}

void loop() {

    // Ohjelman mittaa ja päivittää 6 viimeisimmän arvon keskiarvon, maksimin ja minimin tämän for-silmukan sisällä.
    for (i = 0; i < MITATTAVIEN_ARVOJEN_LKM; i++) {
      
        sensoriArvo = analogRead(SENSORI_PINNI);
        // Summataan luettu sensoriarvo sensoriarvojenSumma -muuttujaan
        sensoriarvojenSumma += sensoriArvo;
        // Lisätään mitattu sensoriarvo taulukkoon, jota käytetään suurimman ja pienimmän mitatun arvon näyttämiseen
        mitatutSensoriArvot[i] = sensoriArvo;

        // Debuggaus tulostelua...
        #ifdef DEBUG
        Serial.print("Yksittainen mitattu arvo ");
        Serial.print(i+1);
        Serial.print("/6: ");
        Serial.println(sensoriArvo);
        #endif

        // Jos kerätty indeksin lukumäärä on 5 (0,1,2,3,4,5), eli vuorossa on mittauskierroksen kuudes mittaus...
        if (i == MITATTAVIEN_ARVOJEN_LKM - 1) {
          
            // Tulostetaan sarjamonitorille 6 viimeisimmän mitatun arvon yhteenlaskettu summa
            Serial.print("Kuuden viimeisimman arvon summa: ");
            Serial.println(sensoriarvojenSumma);
            
            // Tulostetaan sarjamonitorille 6 viimeisimmän mitatun arvon keskiarvo
            Serial.print("Kuuden viimaisimman arvon keskiarvo: ");
            Serial.println(sensoriarvojenSumma / MITATTAVIEN_ARVOJEN_LKM);

            yovalo(sensoriarvojenSumma / MITATTAVIEN_ARVOJEN_LKM);

            // Järjestetään 6 viimeisintä mitattua arvoa nousevaan järjestykseen..
                for (int j = 0; j < MITATTAVIEN_ARVOJEN_LKM; j++) {
                    if (mitatutSensoriArvot[j] >= mitatutSensoriArvot[i]) { // Suoritetaan vertailu
                        tmp = mitatutSensoriArvot[i]; // Talletetaan viimeisin mittaus väliaikaisesti muuttujaan.
                        mitatutSensoriArvot[i] = mitatutSensoriArvot[j]; // Korvataan taulukkoon talletettu sensoriarvo
                        mitatutSensoriArvot[j] = tmp; // Talletetaan viimeisin mittaus uuteen kohtaan taulukossa.
                    }
                }

                // Debuggaus tulostelua...
                #ifdef DEBUG
                Serial.print("Taman mittauskierroksen suurin arvo: ");
                Serial.println(mitatutSensoriArvot[5]);
                Serial.print("Taman mittauskierroksen pienin arvo: ");
                Serial.println(mitatutSensoriArvot[0]);
                #endif
            
            // Jos aiemmin mitattu suurin sensoriarvo on pienempi, kuin 6 viimeisimmän mittauksen suurin arvo, niin päivitetään muuttujaan talletettu suurin sensoriarvo (max).
            if (suurinMitattuArvo < mitatutSensoriArvot[5]) {
                suurinMitattuArvo = mitatutSensoriArvot[5];
            }
            Serial.print("Suurin mitattu arvo: ");
            Serial.println(suurinMitattuArvo);

            // Jos aiemmin mitattu pienin sensoriarvo on suurempi, kuin 6 viimeisimmän mittauksen pienin arvo, niin päivitetään muuttujaan talletettu pienin sensoriarvo (min).
            if (pieninMitattuArvo > mitatutSensoriArvot[0]) {
                pieninMitattuArvo = mitatutSensoriArvot[0];
            }
            Serial.print("Pienin mitattu arvo: ");
            Serial.println(pieninMitattuArvo);

            // Alusttaan mitattujen sensoriarvojen yhteenlaskettu summa takaisin nollaan, koska vuorossa oli mittauskierroksen kuudes mittaus
            sensoriarvojenSumma = 0;
            // Tyhjennetän myös taulukko, johon mittaustulokset oli talletettu
            mitatutSensoriArvot[MITATTAVIEN_ARVOJEN_LKM] = {0};
        }

    }
    // Tämä on vain sen takia, että sarjamonitorille tulostettu data on selkeämmin luettavaa
    Serial.println();
    // Tehdään uusi kuuden mittauksen sarja sekunnin välein
    delay(1000);

}
