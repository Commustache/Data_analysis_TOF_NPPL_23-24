//questo script contiene sicuramente errori e non è stato ottimizzato ai fini della velocità di esecuzione o simili.
//contiene perl tutte le funzioni e i passaggi cruciali per l'analisi dati.

#include <iostream>
#include <fstream>
#include <vector>
#include "TCanvas.h"
#include "TGraph.h"

#define ALPHA 2.355

void GaussianFitEnergyCalibration(TH1I *histogram, int a, int b, int c, int d) {
    histogram->Fit("gaus","","",780,890);
    TF1 *ffit = new TF1("ffit", "gaus(0)+gaus(3)");
    ffit->SetParameter(0,a);
    ffit->SetParameter(1,b);
    ffit->SetParameter(2,1);
    ffit->SetParameter(3,c);
    ffit->SetParameter(4,d);
    ffit->SetParameter(5,1);
    histogram->Fit("ffit","+", "",2070,2118);
}

void FunkFit(TGraphErrors *gr) {
    TF1 *fitfunc1 = new TF1("fitfunc1", "[0]*x + [1]", 0, 7);

    fitfunc1->SetParameter(0, 1E-3);
    fitfunc1->SetParameter(1, 1E-1);
    gr->Fit(fitfunc1, "");
    gr->SetLineColor(kRed);
    gr->SetMarkerColor(kBlue);
    gr->SetMarkerSize(1);
    gr->SetMarkerStyle(20);
    gr->SetTitle("Retta di calibrazione channel-energy");
    gr->GetXaxis()->SetTitle("Channel");
    gr->GetYaxis()->SetTitle("Energy");

    Float_t a = fitfunc1->GetParameter(0);
    Float_t a_err = fitfunc1->GetParError(0);
    Float_t b = fitfunc1->GetParameter(1);
    Float_t b_err = fitfunc1->GetParError(1);

    //calcoli che sono serviti per avere una stima dei valori di a e b
    //Float_t a=(en[1]-en[0])/(ch[1]-ch[0]);
    //Float_t b=en[0]-(ch[0]*((en[1]-en[0])/(ch[1]-ch[0])));

    cout << a << "+/-" << a_err << endl;
    cout << b << "+/-" << b_err << endl;

}

Float_t EnergyfromChannel(Float_t a, Float_t b, Int_t channel) {
    Float_t Energy=(a*channel)+b;

    return Energy;
}

void analisi_tof()
{
    const char *nomeFile="grc_tof_run4.txt";
    std::ifstream inputFile(nomeFile);
    TH1I *hspectrum = new TH1I("hspectrum", "Fit gaussiano picco 2", 4092, 0, 4091);

    if (!inputFile.is_open()) {
        std::cerr << "Impossibile aprire il file " << nomeFile << std::endl;
        return;
    }
    // Leggi il numero di valori dal primo elemento del file
    int n_values;
    inputFile >> n_values;

    // Crea un vettore per contenere i valori
    std::vector<float> count(n_values);

    // Leggi i valori dal file e inseriscili nel vettore
    for (int i = 0; i < n_values; i++) {
        inputFile >> count[i];
        hspectrum->Fill(count[i]);
        //cout << count[i] << endl;
    }

    
    TCanvas* c10 = new TCanvas("c10","c10",700,500);
    hspectrum->Draw(); //disegna
    hspectrum->GetXaxis()->SetRangeUser(2040,2120); //disegna solo il range definito
    hspectrum->GetXaxis()->SetTitle("Energia [ch]");
    hspectrum->GetYaxis()->SetTitle("Conteggi");
    hspectrum->SetLineColor(kBlue);
    hspectrum->SetFillColor(kBlue);
    hspectrum->SetFillStyle(3002);
    hspectrum->SetStats(0); //Rimuove le statistiche dall'istogramma

    cout << "\n" << endl;

    GaussianFitEnergyCalibration(hspectrum,1500, 2075, 7000, 2090);

    Float_t en[2]={2.298, 5.463};
    Float_t ch[2]={833.96, 2093.22};
    Float_t ch_err[2]={0.0902431, 0.0242968};
    //Float_t energy_err[2]={0, 0};

    cout << "\n" << endl;

    TCanvas* c = new TCanvas("c","c",700,500);
    TGraphErrors *gr1 = new TGraphErrors(2, ch, en, ch_err);
    TF1 *fitfunc1 = new TF1("fitfunc1", "[0]*x + [1]", 0, 7); // Definizione della funzione di best-fit (lineare: y = ax + b)

    fitfunc1->SetParameter(0, 1E-3);
    fitfunc1->SetParameter(1, 1E-1);
    gr1->Fit(fitfunc1, "");
    gr1->SetLineColor(kRed);
    gr1->SetMarkerColor(kBlue);
    gr1->SetMarkerSize(1);
    gr1->SetMarkerStyle(20);
    gr1->SetTitle("Retta di calibrazione canale-energia");
    gr1->GetXaxis()->SetTitle("Channel");
    gr1->GetYaxis()->SetTitle("Energy");

    Float_t a = fitfunc1->GetParameter(0);
    Float_t a_err = fitfunc1->GetParError(0);
    Float_t b = fitfunc1->GetParameter(1);
    Float_t b_err = fitfunc1->GetParError(1);

    //calcoli che sono serviti per avere una stima dei valori di a e b
    //Float_t a=(en[1]-en[0])/(ch[1]-ch[0]);
    //Float_t b=en[0]-(ch[0]*((en[1]-en[0])/(ch[1]-ch[0])));

    cout << "a= "<< a << "+/-" << a_err << endl;
    cout << "b= "<< b << "+/-" << b_err << endl;

    //FunkFit(gr1);
    gr1->Draw();
    
    cout << "\n" << endl;

    //RISOLUZIONE ENERGETICA
    const char *nomeFile1="grc_tof_run3.txt";
    std::ifstream inputFile1(nomeFile1);
    TH1I *hspecpulser = new TH1I("hspecpulser", "Spettro in energia con pulser", 4092, 0, 4091);

    if (!inputFile1.is_open()) {
        std::cerr << "Impossibile aprire il file " << nomeFile1 << std::endl;
        return;
    }
    // Leggi il numero di valori dal primo elemento del file
    int n_val;
    inputFile1 >> n_val;

    // Crea un vettore per contenere i valori
    std::vector<float> count1(n_val);

    // Leggi i valori dal file e inseriscili nel vettore
    for (int i = 0; i < n_val; i++) {
        inputFile1 >> count1[i];
        hspecpulser->Fill(count1[i]);
        //cout << count[i] << endl;
    }


    TCanvas* c1 = new TCanvas("c1","c1",700,500);
    hspecpulser->Draw(); //disegna
    hspecpulser->GetXaxis()->SetRangeUser(1540,1580); //disegna solo il range definito
    hspecpulser->GetXaxis()->SetTitle("Energia [ch]");
    hspecpulser->GetYaxis()->SetTitle("Conteggi");
    hspecpulser->SetLineColor(kBlue);
    hspecpulser->SetFillColor(kBlue);
    hspecpulser->SetFillStyle(3002);
    hspecpulser->SetStats(0); //Rimuove le statistiche dall'istogramma

    TF1 *ffitp = new TF1("ffitp", "gaus(0)");
    ffitp->SetParameter(0,1800);
    ffitp->SetParameter(1,1560);
    ffitp->SetParameter(2,1);
    hspecpulser->Fit("ffitp", "", "", 1550, 1570);

    Float_t fwhm= ALPHA*(ffitp->GetParameter(2)); //il parametro 2 è la sigma che mi serve per calcolare la FWHM (il parametro (1) invece è la media)
    Float_t fwhm_err= ALPHA*(ffitp->GetParError(2)); // stesso ragionamento di prima ma per calcolare l'errore

    Float_t En_fwhm_pulser=EnergyfromChannel(a, b, fwhm);
    Float_t En_picco_pulser=EnergyfromChannel(a,b,ffitp->GetParameter(1));
    cout <<"Energia pulser a FWHM --> " << En_fwhm_pulser << endl;

    Float_t R_pulser=(fwhm/En_picco_pulser);
    cout << "Risoluzione --> " << R_pulser << endl;
    //cout << fwhm << " +/- " << fwhm_err << endl;
    
    cout << "\n" << endl;


    //CALIBRAZIONE IN TEMPO E MISURA DEL TOF

    const char *nomeFile2="grc_tof_run5_ch2.txt";
    std::ifstream inputFile2(nomeFile2);
    TH1I *hspectime = new TH1I("hspectime", "Fit gaussiani per lo spettro tempo", 4092, 0, 4091);

    if (!inputFile2.is_open()) {
        std::cerr << "Impossibile aprire il file " << nomeFile2 << std::endl;
        return;
    }
    // Leggi il numero di valori dal primo elemento del file
    int n_valori;
    inputFile2 >> n_valori;

    // Crea un vettore per contenere i valori
    std::vector<float> count2(n_valori);

    // Leggi i valori dal file e inseriscili nel vettore
    for (int i = 0; i < n_valori; i++) {
        inputFile2 >> count2[i];
        hspectime->Fill(count2[i]);
        //cout << count[i] << endl;
    }

    TCanvas* c2 = new TCanvas("c2","c2",700,500);
    hspectime->Draw(); //disegna
    hspectime->GetXaxis()->SetRangeUser(1000,2000); //disegna solo il range definito
    hspectime->GetYaxis()->SetRangeUser(0,150);
    hspectime->GetXaxis()->SetTitle("Tempo [ch]");
    hspectime->GetYaxis()->SetTitle("Conteggi");
    hspectime->SetLineColor(kBlue);
    hspectime->SetFillColor(kBlue);
    hspectime->SetFillStyle(3002);
    hspectime->SetStats(0); //Rimuove le statistiche dall'istogramma

    hspectime->Fit("gaus", "", "", 1200, 1500);
    hspectime->Fit("gaus", "+", "", 1500, 1850);

    cout << "\n" << endl;

    //GRAFICO CALIBRAZIONE TEMPI
    const char *nomeFile3="grc_tof_run2_ch2.txt";
    std::ifstream inputFile3(nomeFile3);
    TH1I *hspectimeres = new TH1I("hspectimeres", "Spettro calibrazione tempi", 4092, 0, 4091);

    if (!inputFile3.is_open()) {
        std::cerr << "Impossibile aprire il file " << nomeFile3 << std::endl;
        return;
    }
    // Leggi il numero di valori dal primo elemento del file
    int n_valori_t;
    inputFile3 >> n_valori_t;

    // Crea un vettore per contenere i valori
    std::vector<float> count3(n_valori_t);

    // Leggi i valori dal file e inseriscili nel vettore

    for (int i = 0; i < n_valori_t; i++) {
        inputFile3 >> count3[i];
        if(count3[i]>850) {
            hspectimeres->Fill(count3[i]);
            //cout << count[i] << endl;
        }
        
    }


    TCanvas* c3 = new TCanvas("c3","c3",700,500);
    hspectimeres->Draw(); //disegna
    hspectimeres->GetXaxis()->SetRangeUser(0,5000); //disegna solo il range definito
    //hspectimeres->GetYaxis()->SetRangeUser(0,150);
    hspectimeres->GetXaxis()->SetTitle("Tempo [ch]");
    hspectimeres->GetYaxis()->SetTitle("Conteggi");
    hspectimeres->SetLineColor(kBlue);
    hspectimeres->SetFillColor(kBlue);
    hspectimeres->SetFillStyle(3002);
    hspectimeres->SetStats(0); //Rimuove le statistiche dall'istogramma

    //hspectimeres->Fit("gaus", "", "", 800, 850);
    hspectimeres->Fit("gaus", "", "", 1284, 1290);
    hspectimeres->Fit("gaus", "+", "", 1732, 1737);
    hspectimeres->Fit("gaus", "+", "", 2194, 2199);
    hspectimeres->Fit("gaus", "+", "", 2628, 2632);
    hspectimeres->Fit("gaus", "+", "", 3062, 3066);
    hspectimeres->Fit("gaus", "+", "", 3463, 3468);
    //se metto "+" nel grafico vengono graficati tutti i fit in contemporanea.


    Float_t ch_t[6]={1.28685e+03, 1.73420e+03, 2.19680e+03, 2.62992e+03, 3.06394e+03, 3.46537e+03}; //centroidi trovati tramite il fit gaussiano
    Float_t ritardo[6]={4,8,12,16,20,24}; //ritardi impostati tramite il modulo passivo, aggiungere quello del modulo attivo 85.5 ns (sono dei delta t)
    Float_t rit_err=0.5; //ns nel calcolo li ho considerati senza errore perche' qualcosa non torna, troppe parentesu :)
    Float_t ch_t_err[6]={7.98742e-01, 7.42826e-01, 7.27873e-01, 7.65057e-01, 7.73559e-01, 9.85587e-01}; //deviazione standard
    Double_t somma=0, peso_somma=0, differenza_errore_te=0;

    for(int i=0; i<5; i++)
    {
        Float_t alpha=(ritardo[i+1]-ritardo[i])/(ch_t[i+1]-ch_t[i]);
        //cout << "differenza picco t2= "<< ritardo[i+1] << " e t1= "<< ritardo[i] << "--> alpha= "<<alpha << endl;

        differenza_errore_te=sqrt((ch_t_err[i+1]*ch_t_err[i+1])+(ch_t_err[i]*ch_t_err[i]));

        Float_t errore_alpha=differenza_errore_te*((ritardo[i+1]-ritardo[i])/pow((ch_t[i+1]-ch_t[i]),2));

        //MEDIA PESATA
        double peso=1/(errore_alpha*errore_alpha);
        double prodotto= peso*alpha;
        somma=somma+prodotto;
        peso_somma=peso_somma+peso;

    }

    Float_t intercetta=somma/peso_somma;
    Float_t intercetta_errore=1/(sqrt(peso_somma));
    
    cout << "\nValore coeff. angolare: " << intercetta << " +/- " << intercetta_errore << " ns/Ch \n" <<endl;

    //CALIBRAZIONE IN TEMPO E MISURA DEL TOF
    Float_t ch_tof_teorico[2]={1347.1, 1701.9};
    Float_t ch_tof_teorico_dev[2]={5.5 , 5.2};
    Float_t distanza=0.078, massa_alfa=3728.4; //la massa e' in MeV/c^2

    //calcolo TOF teorico
    Double_t somma_b=0, peso_somma_b=0;
    Double_t somma_b_m=0, peso_somma_b_m=0;
    Float_t delay=85.5; //ns
    Float_t tm_errore=sqrt((0.1*0.1)+(0.5*0.5));

    for(int i=0; i<2; i++)
    {
        Float_t tof_teorico=sqrt(massa_alfa /(en[i]*2))*(distanza/3E8)*1E9;
        cout << "Il valore teorico del TOF per l'energia di " << en[i] << " MeV e' " << tof_teorico << " +/- 0.1 ns." <<endl;

        //cacolo termine noto della logica non inversa
        Float_t b_primo=tof_teorico+(intercetta*ch_tof_teorico[i]);
        //cout << "Il valore del termine noto per l'energia di " << en[i] << " MeV e' " << b_primo <<endl;

        Float_t errore_b_primo=sqrt(pow((intercetta_errore*ch_tof_teorico[i]),2)+pow(ch_tof_teorico_dev[i]*intercetta,2));

        //MEDIA PESATA di b_primo
        double peso_b=1/(errore_b_primo*errore_b_primo);
        double prodotto_b= peso_b*b_primo;
        somma_b=somma_b+prodotto_b;
        peso_somma_b=peso_somma_b+peso_b;

        //Trovo t_m come la differenza tra il delay e il tof teorico
        cout << "Valore di t_m del "<< i+1 << " picco: "<< delay-tof_teorico << " +/- " << tm_errore << " ns." <<endl;

        //cacolo termine noto della logica inversa
        Float_t b_m=(delay-tof_teorico)-(intercetta*ch_tof_teorico[i]);
        cout << "Il valore del termine noto per l'energia di " << en[i] << " MeV e' " << b_m <<endl;

        Float_t errore_b_m=sqrt(pow((intercetta_errore*ch_tof_teorico[i]),2)+pow(ch_tof_teorico_dev[i]*intercetta,2)+(tm_errore*tm_errore));

        //MEDIA PESATA di b_m
        double peso_b_m=1/(errore_b_m*errore_b_m);
        double prodotto_b_m= peso_b_m*b_m;
        somma_b_m=somma_b_m+prodotto_b_m;
        peso_somma_b_m=peso_somma_b_m+peso_b_m;

    }
    
    Float_t b_primo_media= somma_b/peso_somma_b;
    cout << "\nValore termine noto logica non inversa b': " << b_primo_media << " +/- " << 1/(sqrt(peso_somma_b))<<endl;
    cout << "Valore termine noto logica inversa b_m: " << somma_b_m/peso_somma_b_m << " +/- " << 1/(sqrt(peso_somma_b_m)) << "\n" <<endl;

    //Troviamo i valori del Tof sperimentali tramite la retta di calibrazione
    for(int i=0; i<2; i++)
    {
        Double_t tof_sperimentale=-(intercetta*ch_tof_teorico[i])+b_primo_media;
        cout << "Il valore sperimentale del TOF per l'energia di " << en[i] << " MeV e' " << tof_sperimentale << " +/- 0.1 ns." <<endl;

    }

    //RISOLUZIONE TEMPO


    //ACQUISIZIONE MULTIPARAMETRICA

    TH2D* hmulti = new TH2D("hmulti", "Acquisizione multiparametrica", 4092, 0, 4091, 4092, 0, 4091);

    const char *nomeFile2_1="grc_tof_run5_ch1.txt";
    std::ifstream inputFile2_1(nomeFile2_1);

    if (!inputFile2_1.is_open()) {
        std::cerr << "Impossibile aprire il file " << nomeFile2_1 << std::endl;
        return;
    }

    // Leggi il numero di valori dal primo elemento del file
    int n_valoris;
    inputFile2_1 >> n_valoris;
    
    if(n_valoris==n_valori) n_valoris=n_valori;

    // Crea un vettore per contenere i valori
    std::vector<float> count2_1(n_valori);

    for (int i = 0; i < n_valori; i++) {
        inputFile2_1 >> count2_1[i];
        hmulti->Fill(count2[i],count2_1[i]);
        //cout << count[i] << endl;
    }

    TCanvas *c4=new TCanvas("c4","c4",700,500);
    hmulti->Draw("COLZ");
    //hmulti->Draw("LEGO2"); //disegna
    hmulti->GetXaxis()->SetRangeUser(500,2200);
    hmulti->GetYaxis()->SetRangeUser(0,2500);
    hmulti->GetXaxis()->SetTitle("Tempo [ch]");
    hmulti->GetYaxis()->SetTitle("Energia [ch]");
    hmulti->GetZaxis()->SetRangeUser(0, 5); //Imposta il range di valori di z
    hmulti->SetStats(0); //Rimuove le statistiche dall'istogramma

 //   TGraph *gr2 = new TGraph(2, ch_tof_teorico, ch);
 //   int nPoints = 0; 
//
 //   //riempi TGraph con i dati dell'histo
 //   for(int i=1; i<=hmulti->GetNbinsX(); i++) {
 //       for (int j=1; j<=hmulti->GetNbinsY(); j++) {
 //           double x = hmulti->GetXaxis()->GetBinCenter(i);
 //           double y = hmulti->GetYaxis()->GetBinCenter(j);
 //           double z = hmulti->GetBinContent(i,j);
//
 //           //aggiungo i dati al TGraph
 //           //gr2->SetPoint(nPoints, x, y, z);
 //           gr2->SetPoint(nPoints, x, y);
 //           nPoints++;
 //       }
 //   }
 //   TF1 *fmulti = new TF1("fmulti", "[0]/(x*x)");
//
 //   fmulti->SetParameter(0,1e-16);
//
 //   gr2->Fit("fmulti", "Q");
 //   gr2->Draw("");
//
//
//
 //   
//
 //   
 //   
 //    
}//
//
