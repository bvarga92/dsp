# Rendszeridentifik�ci� LMS �s NLMS algoritmussal

![1](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_0.png)

- **1. eset**: A b�tors�gi �s regulariz�ci�s param�terek �gy vannak be�ll�tva, hogy egys�gnyi zajteljes�tm�ny eset�n a k�t algoritmus nagyj�b�l azonos sebess�ggel konverg�ljon.

![2](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_1_unit_variance.png)

- **2. eset**: Az algoritmusok param�terei v�ltozatlanok, de a zajteljes�tm�ny az 1. esethez k�pest a fel�re cs�kkent. Az LMS lassabban, az NLMS v�ltozatlan sebess�ggel konverg�l.

![3](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_2_half_variance.png)

- **3. eset**: Az algoritmusok param�terei v�ltozatlanok, de a zajteljes�tm�ny az 1. esethez k�pest a dupl�j�ra n�tt. Az LMS gyorsabban, az NLMS v�ltozatlan sebess�ggel konverg�l.

![4](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_3_double_variance.png)

Az �tvitel identifik�ci�ja �s FIR sz�r�k�nt t�rt�n� modellez�se mindk�t m�dszerrel sikeres volt:

![5](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_4_tf.png)


# Akt�v zajcs�kkent�s (ANC) FxLMS algoritmussal

![6](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_0.png)

A m�sodlagos �t (beavatkoz� hangsz�r� --> hibamikrofon) identifik�ci�ja az el�z� algoritmusok valamelyik�vel t�rt�nhet, majd ennek felhaszn�l�s�val az FxLMS algoritmus identifik�lja az els�dleges utat (zajforr�s --> hibamikrofon), �s elv�gzi a kiolt�st:

![7](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_1_sin.png)
![8](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_3_saw.png)
![9](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_5_gauss.png)

Megfigyelhet�, hogy az FxLMS algoritmus megtanulja az akusztikus csatorna �tvitel�t a relev�ns frekvenci�kon (periodikus gerjeszt�s esetben a harmonikus komponensek frekvenci�in, feh�r zaj eset�n mindenhol):

![10](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_2_sin.png)
![11](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_4_saw.png)
![12](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_6_gauss.png)


# Adapt�v vonaljav�t�s (ALE)

![13](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main3_0.png)

A bemen� jel egy sztochasztikus (pl. besz�d, Gauss-zaj) �s egy periodikus (pl. h�l�zati 50 Hz-es zavar) komponenst tartalmaz. A periodikus komponens �nmaga k�sleltetett v�ltozat�val mindig korrel�lt, azonban a sztochasztikus komponens kell�en nagy k�sleltet�s eset�n m�r korrel�latlan �nmag�val (v�gtelen s�vsz�less�g� feh�r zaj autokorrel�ci�s f�ggv�nye Dirac-delta), �gy a kompoenensek sz�tv�laszthat�k:

![14](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main3_1.png)


# Adapt�v visszhangcs�kkent�s (AEC)

![15](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main4_0.png)

A visszhangra vonatkoz� �tvitel feh�r zajjal t�rt�n� identifik�ci�ja ut�n a visszhang a v�teli oldalon becs�lhet�, �gy a vett jelb�l kivonhat�:

![16](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main4_1.png)
