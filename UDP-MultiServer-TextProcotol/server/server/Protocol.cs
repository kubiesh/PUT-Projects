using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace server
{
    [Flags]
    public enum Operacja
    {
        Nawiaz, Potwierdz, CzasDoKonca, Zgadnij, CzyOdgadnieta, Start, Stop, Rozlacz,
    }
    [Flags]
    public enum Odpowiedz
    {
        OK,JuzRozpoczeto,Koniec,BladSerwera,Tak,Nie,ZaDuza,ZaMala,Rozlacz,
    }
   public  class Protocol
    {
        String CZ; //czas
        int ID; //id sesji
        int NS; //numer sekwencyjny
        Operacja OP; //operacja
        Odpowiedz OD; //odpowiedz
        int DA; //dane

        public Protocol() {
            CZ = null;
            ID = 0;
            NS = 0;
            OP = Operacja.Zgadnij;
            OD = Odpowiedz.OK;
            DA = 0;
            
        }
        
        public void SetOP(Operacja _operation)
        {
            OP = _operation;
        }
        public void SetOD(Odpowiedz _odpowiedz)
        {
            OD = _odpowiedz;
        }
        public void SetID(int _SessionID)
        {
            ID=_SessionID;
        }
        public void SetNS(int _nrSekw)
        {
            NS=_nrSekw;
        }
        public void SetDA(int _dane)
        {
            DA = _dane;
        }
        public int GetDA()
        {
            return DA;
        }
        public int GetNS()
        {
            return NS;
        }
        public int GetID()
        {
            return ID;
        }
        public Operacja GetOP()
        {
            return OP;
        }
        public String GetCZ() {
            return CZ;
        }
        public Odpowiedz GetOD() {
            return OD;
        }
        public void OperacjaToString(String op) {
            if (Operacja.Nawiaz.ToString().Equals(op)) {
                this.OP = Operacja.Nawiaz;
            }
            if (Operacja.CzasDoKonca.ToString().Equals(op))
            {
                this.OP = Operacja.CzasDoKonca;
            }
            if (Operacja.Zgadnij.ToString().Equals(op))
            {
                this.OP = Operacja.Zgadnij;
            }
            if (Operacja.Potwierdz.ToString().Equals(op))
            {
                this.OP = Operacja.Potwierdz;
            }
            if (Operacja.CzyOdgadnieta.ToString().Equals(op))
            {
                this.OP = Operacja.CzyOdgadnieta;
            }
            if (Operacja.Start.ToString().Equals(op))
            {
                this.OP = Operacja.Start;
            }
            if (Operacja.Stop.ToString().Equals(op))
            {
                this.OP = Operacja.Stop;
            }
            if (Operacja.Rozlacz.ToString().Equals(op))
            {
                this.OP = Operacja.Rozlacz;
            }


        }
        public void OdpowiedzToString(String odp)
        {
            if (Odpowiedz.OK.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.OK;
            }
            if (Odpowiedz.Tak.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.Tak;
            }
            if (Odpowiedz.Nie.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.Nie;
            }
            if (Odpowiedz.ZaDuza.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.ZaDuza;
            }
            if (Odpowiedz.ZaMala.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.ZaMala;
            }
            if (Odpowiedz.Koniec.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.Koniec;
            }
            if (Odpowiedz.BladSerwera.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.BladSerwera;
            }
            if (Odpowiedz.JuzRozpoczeto.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.JuzRozpoczeto;
            }
            if (Odpowiedz.Rozlacz.ToString().Equals(odp))
            {
                this.OD = Odpowiedz.Rozlacz;
            }
        }

        public void SetAckowlegement(int nrSekw)
        {
            this.OP = Operacja.Potwierdz;
            this.NS = nrSekw;

        }

        public byte[] Serialize() {
            //CZ<<NS<<ID<<OP<<OD
            //Zamienienie slownego komunikatu na bajty
          
            DateTime aktualnadata= DateTime.Now;
            String s_aktualnadata = aktualnadata.Day + "-" + aktualnadata.Month + "-" + aktualnadata.Year.ToString()+" "+ aktualnadata.Hour+":"+aktualnadata.Minute+":"+ aktualnadata.Second;

            String poleczasu = "CZ?" + s_aktualnadata+"<<";
            String poleid = "ID?" + this.ID.ToString() + "<<";
            String polenrsekw = "NS?" + this.NS.ToString() + "<<";
            String poleoperacji = "OP?" + this.OP.ToString() + "<<";
            String poleodpowiedzi = "OD?" + this.OD.ToString() + "<<";
            String poledanych = "DA?" + this.DA.ToString() + "<<";

            String komunikat=poleczasu+poleid+polenrsekw+poleoperacji+poleodpowiedzi+poledanych;
            byte[] serialized = Encoding.ASCII.GetBytes(komunikat);
            return serialized;
        }
        public void Deserialize(byte[] pakiet)
        {
            String komunikat= Encoding.ASCII.GetString(pakiet, 0, pakiet.Length);
            Regex rgx = new Regex(@"\?[\w]+");
            Regex rgxdata = new Regex(@"\?[0-9]{1,2}\-[0-9]{1,2}\-[0-9]{4}\s[0-9]{1,2}\:[0-9]{1,2}\:[0-9]{1,2}");
            MatchCollection matches = rgx.Matches(komunikat);
            MatchCollection match_data = rgxdata.Matches(komunikat);

            if (matches.Count > 0) {
                this.CZ = match_data[0].ToString().Substring(1); //CZ
                 Int32.TryParse(matches[1].ToString().Substring(1), out this.ID); //ID Konwersja String na Int
                Int32.TryParse(matches[2].ToString().Substring(1), out this.NS); //NS
                String op = matches[3].ToString().Substring(1); //OP
                this.OperacjaToString(op);
                String odp = matches[4].ToString().Substring(1); ; //OD
                this.OdpowiedzToString(odp);
                Int32.TryParse(matches[5].ToString().Substring(1), out this.DA); //DA
            }

        }
    }
}
