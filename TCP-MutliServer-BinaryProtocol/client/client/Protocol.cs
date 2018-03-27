using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;




namespace server
{

    /*
    * ================================================================================================
    * KLASA PROTOCOL
    * ODPOWIADA ZA STRUKTURE PAKIETOW WYSYLANYCH ZA POSREDNICTWEM NASZEGO PROTOKOLU
    * ================================================================================================
    */
    class Protocol
    {
        /*
        * ================================================================================================
        * POLA ENUM - OZNACZANE JAKO [FLAGS]
        * [FLAGS] DAJE TO ZE JAK DAMY Operacja.ODEJMOWANIE.ToString() to zwroci 'ODEJMOWANIE' zamiast '0'
        * ================================================================================================
        */
        [Flags]
        public enum Operacja : byte
        {
            ODEJMOWANIE = 0, DZIELENIE = 1, DODAWANIE = 2, SREDNIA = 3,
        }
        [Flags]
        public enum Status : byte
        {
            OK = 0, OVERRANGE = 1, UNDERRANGE = 2, DIVISION0 = 3,
        }

        /*
         * ================================================================================================
         * DEKLARACJE
         * ================================================================================================
         */
        Operacja operation = Operacja.ODEJMOWANIE; //2 bits
        private int number1 = 0; //32 bits
        private int number2 = 0; //32 bits
        private int number3 = 0; //32 bits
        Status state = Status.OK; //4 bits
        private byte sessionID = 0; //6 bits
        bool init = false; //1 bit
        //3 unused bits
        private int packetSize = 14;
        /*
        * ================================================================================================
        * KONIEC DEKLARACJI
        * ================================================================================================
        */



        /*
        * ================================================================================================
        * KONSTRUKTOR
        * ================================================================================================
        */
        public Protocol()
        {
            //Same wartosci domyslne, wyzerowane
            number1 = 0;
            number2 = 0;
            number3 = 0;
            state = Status.OK;
            init = false;
            sessionID = 0;
            operation = Operacja.ODEJMOWANIE;
        }

        /*
        * ================================================================================================
        * KONSTRUKTOR - KONIEC
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * SETTERY
        * ================================================================================================
        */
        public void SetNumber1(int _number1)
        {
            number1 = _number1;
        }
        public void SetNumber2(int _number2)
        {
            number2 = _number2;
        }
        public void SetNumber3(int _number3)
        {
            number3 = _number3;
        }
        public void SetStatus(Status _state)
        {
            state = _state;
        }
        public void SetOperation(Operacja _operation)
        {
            operation = _operation;
        }
        public void SetSessionID(byte _sessionID)
        {
            sessionID = _sessionID;
        }
        public void SetInit(bool _init)
        {
            init = _init;
        }
        /*
        * ================================================================================================
        * SETTERY - KONIEC
        * ================================================================================================
        */

        /*
        * ================================================================================================
        * GETTERY
        * ================================================================================================
        */
        public int GetNumber1()
        {
            return number1;
        }
        public int GetNumber2()
        {
            return number2;
        }
        public int GetNumber3()
        {
            return number3;
        }
        public Status GetStatus()
        {
            return state;
        }
        public Operacja GetOperation()
        {
            return operation;
        }
        public byte GetSessionID()
        {
            return sessionID;
        }
        public bool GetInit()
        {
            return init;
        }

        /*
        * ================================================================================================
        * GETTERY - KONIEC
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * FUNKCJA SERIALIZE
        * ZAKODOWUJE DANE DO POSTACI BINARNEJ
        * (BAJTY SA JUZ W POSTACI BINARNEJ (8bitow), ALE ZEBY UZYSKAC INTERESUJACE NAS WARTOSCI TRZEBA PRZESUWAC POJEDYNCZO
        * BITY WZDLUZ DANEGO BAJTU)
        * NIE JEST TO BYC MOZE NAJBARDZIEJ ELEGANCKIE ROZWIAZANIE, ALE DZIALA I INNEGO NIE WYMYSLILEM WTEDY
        * MOZE TO STRASZNIE WYGLADA, ALE PO ZROZUMIENIU JEDNEJ LINIJKI ZROZUMIESZ WSZYSTKO
        * ================================================================================================
        */
        public byte[] serialize()
        {
            //Tworzymy pakiet ktory trzeba zakodowac
            byte[] pakiet = new byte[packetSize];
            //wyzerowanie
            for (int i = 0; i < packetSize; i++)
            {
                pakiet[i] = 0;
            }

            bool[] poleoperacji = ByteToBoolean((byte)operation); //istotny tylko indeks 0 i 1
            //Int32 numerbituwbajcie = 7;
            //
            // PRZESUNIECIE BITOWE!!!
            // BARDZO WAZNE!
            // MAMY PAKIET[0] CZYLI PIERWSZE 8 BITOW, 1 BAJT
            // ZOSTAL ON WCZESNIEJ WYZEROWANY, CZYLI MA WARTOSC 0b00000000 albo 0 w dziesietnym
            // CALY HACZYK POLEGA NA SUMOWANIU NASZEGO BAJTU Z JEDYNKAMI BADZ ZERAMI
            // poleoperacji[1] jest boolem, sprawdzamy jego wartosc
            // jesli jest true, to przesuwamy o 7 bitow jedynke z miejsca 0, czyli na ostatni bit po lewej poleci jedynka
            // wtedy pakiet[0] = 0b1000000, w szesnastkowym jest to 0x80, w dziesietnym 128
            // dlatego przesuwamy bitowo o iles miejsc, bo ciezko to zrobic w dziesietnym czy szesnastkowym
            // jesli false to caly bajt jest sumowany z serem, czyli nie zmienia swojej wartosci
            // 
            //
            pakiet[0] |= (byte)(poleoperacji[1] ? 1 << 7 : 0); //1szy bit operacji
            pakiet[0] |= (byte)(poleoperacji[0] ? 1 << 6 : 0); //2gi bit operacji
            bool[] liczba1 = IntToBoolean(number1);
            int indeks1 = 31;
            for (int i = 5; i >= 0; i--)
            {
                pakiet[0] |= (byte)(liczba1[indeks1] ? 1 << i : 0); //6bitow pierwszej liczby
                indeks1--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[1] |= (byte)(liczba1[indeks1] ? 1 << i : 0); //8 kolejnych bitow pierwszej liczby
                indeks1--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[2] |= (byte)(liczba1[indeks1] ? 1 << i : 0); //8 kolejnych bitow pierwszej liczby
                indeks1--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[3] |= (byte)(liczba1[indeks1] ? 1 << i : 0); //8 kolejnych bitow pierwszej liczby
                indeks1--;
            }

            pakiet[4] |= (byte)(liczba1[indeks1] ? 1 << 7 : 0); //2 ostatnie pierwszej liczby
            indeks1--;
            pakiet[4] |= (byte)(liczba1[indeks1] ? 1 << 6 : 0); //2 ostatnie pierwszej liczby
            indeks1--;

            //
            //DRUGA LICZBA
            //
            bool[] liczba2 = IntToBoolean(number2);
            int indeks2 = 31;
            for (int i = 5; i >= 0; i--)
            {
                pakiet[4] |= (byte)(liczba2[indeks2] ? 1 << i : 0); //6bitow drugiej liczby
                indeks2--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[5] |= (byte)(liczba2[indeks2] ? 1 << i : 0); //8 kolejnych bitow drugiej liczby
                indeks2--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[6] |= (byte)(liczba2[indeks2] ? 1 << i : 0); //8 kolejnych bitow drugiej liczby
                indeks2--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[7] |= (byte)(liczba2[indeks2] ? 1 << i : 0); //8 kolejnych bitow drugiej liczby
                indeks2--;
            }
            //2 ostatnie bity drugiej liczby
            pakiet[8] |= (byte)(liczba2[indeks2] ? 1 << 7 : 0); 
            indeks2--;
            pakiet[8] |= (byte)(liczba2[indeks2] ? 1 << 6 : 0);
            indeks2--;
            //
            //TRZECIA LICZBA
            //
            bool[] liczba3 = IntToBoolean(number3);
            int indeks3 = 31;
            for (int i = 5; i >= 0; i--)
            {
                pakiet[8] |= (byte)(liczba3[indeks3] ? 1 << i : 0); //6bitow trzeciej liczby
                indeks3--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[9] |= (byte)(liczba3[indeks3] ? 1 << i : 0); //8 kolejnych bitow trzeciej liczby
                indeks3--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[10] |= (byte)(liczba3[indeks3] ? 1 << i : 0); //8 kolejnych bitow trzeciej liczby
                indeks3--;
            }
            for (int i = 7; i >= 0; i--)
            {
                pakiet[11] |= (byte)(liczba3[indeks3] ? 1 << i : 0); //8 kolejnych bitow trzeciej liczby
                indeks3--;
            }
            //2 ostatnie trzeciej liczby
            pakiet[12] |= (byte)(liczba3[indeks3] ? 1 << 7 : 0); 
            indeks3--;
            pakiet[12] |= (byte)(liczba3[indeks3] ? 1 << 6 : 0);
            indeks3--;

            bool[] polestatusu = ByteToBoolean((byte)state); //istotny tylko indeks 3,2,1,0
            int indeks4 = 3;
            for (int i = 5; i >= 2; i--)
            {
                pakiet[12] |= (byte)(polestatusu[indeks4] ? 1 << i : 0);
                indeks4--;
            }
            bool[] poleindentyfikatora = ByteToBoolean(sessionID);
            int indeks5 = 5;
            for (int i = 1; i >= 0; i--)
            {
                pakiet[12] |= (byte)(poleindentyfikatora[indeks5] ? 1 << i : 0);
                indeks5--;
            }
            for (int i = 7; i >= 4; i--)
            {
                pakiet[13] |= (byte)(poleindentyfikatora[indeks5] ? 1 << i : 0);
                indeks5--;
            }
            pakiet[13] |= (byte)(init ? 1 << 3 : 0); //bit init



            return pakiet;
        }
        /*
        * ================================================================================================
        * FUNKCJA SERIALIZE - KONIEC
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * FUNKCJA DESERIALIZE
        * ANALOGICZNIE DO SERIALIZE, ODKODOWUJE DANE I ZAPISUJE JE W STRUKTURZE, ABY NASZ PROGRAM MOGL JE ZROZUMIEC
        * ================================================================================================
        */
        public void deserialize(byte[] pakiet)
        {
            bool[] binpakiet0 = ByteToBoolean(pakiet[0]);
            byte _operation = new byte();
            _operation = 0;
            _operation |= (byte)(binpakiet0[7] ? 1 << 1 : 0); //1szy bit operacji
            _operation |= (byte)(binpakiet0[6] ? 1 << 0 : 0); //2gi bit operacji
            operation = (Operacja)_operation;
            int liczba1 = 0;
            int indeks1 = 31;
            for (int i = 5; i >= 0; i--)
            {
                liczba1 |= (binpakiet0[i] ? 1 << indeks1 : 0);
                indeks1--;
            }
            bool[] binpakiet1 = ByteToBoolean(pakiet[1]);
            for (int i = 7; i >= 0; i--)
            {
                liczba1 |= (binpakiet1[i] ? 1 << indeks1 : 0);
                indeks1--;
            }
            bool[] binpakiet2 = ByteToBoolean(pakiet[2]);
            for (int i = 7; i >= 0; i--)
            {
                liczba1 |= (binpakiet2[i] ? 1 << indeks1 : 0);
                indeks1--;
            }
            bool[] binpakiet3 = ByteToBoolean(pakiet[3]);
            for (int i = 7; i >= 0; i--)
            {
                liczba1 |= (binpakiet3[i] ? 1 << indeks1 : 0);
                indeks1--;
            }
            bool[] binpakiet4 = ByteToBoolean(pakiet[4]);
            for (int i = 7; i >= 6; i--)
            {
                liczba1 |= (binpakiet4[i] ? 1 << indeks1 : 0);
                indeks1--;
            }
            int liczba2 = 0;
            int indeks2 = 31;
            for (int i = 5; i >= 0; i--)
            {
                liczba2 |= (binpakiet4[i] ? 1 << indeks2 : 0);
                indeks2--;
            }
            bool[] binpakiet5 = ByteToBoolean(pakiet[5]);
            for (int i = 7; i >= 0; i--)
            {
                liczba2 |= (binpakiet5[i] ? 1 << indeks2 : 0);
                indeks2--;
            }
            bool[] binpakiet6 = ByteToBoolean(pakiet[6]);
            for (int i = 7; i >= 0; i--)
            {
                liczba2 |= (binpakiet6[i] ? 1 << indeks2 : 0);
                indeks2--;
            }
            bool[] binpakiet7 = ByteToBoolean(pakiet[7]);
            for (int i = 7; i >= 0; i--)
            {
                liczba2 |= (binpakiet7[i] ? 1 << indeks2 : 0);
                indeks2--;
            }
            bool[] binpakiet8 = ByteToBoolean(pakiet[8]);
            for (int i = 7; i >= 6; i--)
            {
                liczba2 |= (binpakiet8[i] ? 1 << indeks2 : 0);
                indeks2--;
            }
            int liczba3 = 0;
            int indeks3 = 31;
            for (int i = 5; i >= 0; i--)
            {
                liczba3 |= (binpakiet8[i] ? 1 << indeks3 : 0);
                indeks3--;
            }
            bool[] binpakiet9 = ByteToBoolean(pakiet[9]);
            for (int i = 7; i >= 0; i--)
            {
                liczba3 |= (binpakiet9[i] ? 1 << indeks3 : 0);
                indeks3--;
            }
            bool[] binpakiet10 = ByteToBoolean(pakiet[10]);
            for (int i = 7; i >= 0; i--)
            {
                liczba3 |= (binpakiet10[i] ? 1 << indeks3 : 0);
                indeks3--;
            }
            bool[] binpakiet11 = ByteToBoolean(pakiet[11]);
            for (int i = 7; i >= 0; i--)
            {
                liczba3 |= (binpakiet11[i] ? 1 << indeks3 : 0);
                indeks3--;
            }
            bool[] binpakiet12 = ByteToBoolean(pakiet[12]);
            for (int i = 7; i >= 6; i--)
            {
                liczba3 |= (binpakiet12[i] ? 1 << indeks3 : 0);
                indeks3--;
            }
            byte _state = new byte();
            _state = 0;
            _state |= (byte)(binpakiet12[5] ? 1 << 3 : 0);
            _state |= (byte)(binpakiet12[4] ? 1 << 2 : 0);
            _state |= (byte)(binpakiet12[3] ? 1 << 1 : 0);
            _state |= (byte)(binpakiet12[2] ? 1 << 0 : 0);
            state = (Status)_state;
            byte _sessionID = new byte();
            _sessionID |= (byte)(binpakiet12[1] ? 1 << 5 : 0);
            _sessionID |= (byte)(binpakiet12[0] ? 1 << 4 : 0);
            bool[] binpakiet13 = ByteToBoolean(pakiet[13]);
            _sessionID |= (byte)(binpakiet13[7] ? 1 << 3 : 0);
            _sessionID |= (byte)(binpakiet13[6] ? 1 << 2 : 0);
            _sessionID |= (byte)(binpakiet13[5] ? 1 << 1 : 0);
            _sessionID |= (byte)(binpakiet13[4] ? 1 << 0 : 0);
            init = binpakiet13[3];

            number1 = liczba1;
            number2 = liczba2;
            number3 = liczba3;
            sessionID = _sessionID;
        }

        /*
        * ================================================================================================
        * FUNKCJA DESERIALIZE - KONIEC
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * FUNKCJA BYTETOBOOLEAN
        * KONWERTUJE TYP BYTE NA 8-ELEMENTOWA TABLICE BOOLI, ODPOWIADAJACA POSZCZEGOLNYM BITOM W 8-BITOWYM BYTE
        * ================================================================================================
        */
        public static bool[] ByteToBoolean(byte source)
        {
            var bitArray = new BitArray(new[] { source });
            var target = new bool[8];
            bitArray.CopyTo(target, 0);
            return target;
        }
        /*
        * ================================================================================================
        * FUNKCJA BYTETOBOOLEAN - KONIEC
        * ================================================================================================
        */

        /*
        * ================================================================================================
        * FUNKCJA INTTOBOOLEAN
        * KONWERTUJE TYP INT NA 32-ELEMENTOWA TABLICE BOOLI, ODPOWIADAJACA POSZCZEGOLNYM BITOM W 32-BITOWYM INT
        * ================================================================================================
        */
        public static bool[] IntToBoolean(int source)
        {
            var bitArray = new BitArray(new[] { source });
            var target = new bool[32];
            bitArray.CopyTo(target, 0);
            return target;
        }


    }
}
