# Rendszeridentifikáció LMS és NLMS algoritmussal

![1](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_0.png)

- **1. eset**: A bátorsági és regularizációs paraméterek úgy vannak beállítva, hogy egységnyi zajteljesítmény esetén a két algoritmus nagyjából azonos sebességgel konvergáljon.

![2](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_1_unit_variance.png)

- **2. eset**: Az algoritmusok paraméterei változatlanok, de a zajteljesítmény az 1. esethez képest a felére csökkent. Az LMS lassabban, az NLMS változatlan sebességgel konvergál.

![3](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_2_half_variance.png)

- **3. eset**: Az algoritmusok paraméterei változatlanok, de a zajteljesítmény az 1. esethez képest a duplájára nõtt. Az LMS gyorsabban, az NLMS változatlan sebességgel konvergál.

![4](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_3_double_variance.png)

Az átvitel identifikációja és FIR szûrõként történõ modellezése mindkét módszerrel sikeres volt:

![5](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main1_4_tf.png)


# Aktív zajcsökkentés (ANC) FxLMS algoritmussal

![6](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_0.png)

A másodlagos út (beavatkozó hangszóró --> hibamikrofon) identifikációja az elõzõ algoritmusok valamelyikével történhet, majd ennek felhasználásával az FxLMS algoritmus identifikálja az elsõdleges utat (zajforrás --> hibamikrofon), és elvégzi a kioltást:

![7](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_1_sin.png)
![8](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_3_saw.png)
![9](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_5_gauss.png)

Megfigyelhetõ, hogy az FxLMS algoritmus megtanulja az akusztikus csatorna átvitelét a releváns frekvenciákon (periodikus gerjesztés esetben a harmonikus komponensek frekvenciáin, fehér zaj esetén mindenhol):

![10](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_2_sin.png)
![11](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_4_saw.png)
![12](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main2_6_gauss.png)


# Adaptív vonaljavítás (ALE)

![13](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main3_0.png)

A bemenõ jel egy sztochasztikus (pl. beszéd, Gauss-zaj) és egy periodikus (pl. hálózati 50 Hz-es zavar) komponenst tartalmaz. A periodikus komponens önmaga késleltetett változatával mindig korrelált, azonban a sztochasztikus komponens kellõen nagy késleltetés esetén már korrelálatlan önmagával (végtelen sávszélességû fehér zaj autokorrelációs függvénye Dirac-delta), így a kompoenensek szétválaszthatók:

![14](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main3_1.png)


# Adaptív visszhangcsökkentés (AEC)

![15](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main4_0.png)

A visszhangra vonatkozó átvitel fehér zajjal történõ identifikációja után a visszhang a vételi oldalon becsülhetõ, így a vett jelbõl kivonható:

![16](https://raw.githubusercontent.com/bvarga92/dsp/main/adaptive/img/main4_1.png)
