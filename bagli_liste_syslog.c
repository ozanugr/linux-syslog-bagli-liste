#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- SYSLOG DÜÐÜM YAPISI ---
 * Verileri tek bir metin yerine, Syslog standartlarýna uygun 
 * olarak parçalayarak (Zaman, Seviye, Mesaj) tutuyoruz.
 */
struct SyslogDugumu {
    int id;
    char zaman[50];
    char seviye[20];
    char mesaj[200];
    struct SyslogDugumu* sonraki;
};

// Listenin baþýný ve sonunu tutan iþaretçiler (Pointers)
struct SyslogDugumu* ilk_dugum = NULL;
struct SyslogDugumu* son_dugum = NULL; // Hýzlý ekleme (O(1)) için kuyruk iþaretçisi
int log_sayaci = 1;

/* * 1. ADIM: OTOMATÝK TEST DOSYASI OLUÞTURMA
 * Programýn Dev-C++'ta sorunsuz çalýþmasý için kendi log dosyasýný 
 * (syslog_verileri.txt) otomatik olarak yaratmasýný saðlýyoruz.
 * Windows Terminali Türkçe karakterlerde hata verdiði için 
 * BÝLGÝ yerine BILGI kullanýlmýþtýr.
 */
void test_dosyasi_olustur() {
    FILE* dosya = fopen("syslog_verileri.txt", "w");
    if (dosya != NULL) {
        fprintf(dosya, "10:45:01,BILGI,Sistem basariyla onyuklendi.\n");
        fprintf(dosya, "10:45:05,UYARI,USB cihazi algilandi ancak taninmadi.\n");
        fprintf(dosya, "10:46:00,HATA,Veritabani baglantisi koptu!\n");
        fprintf(dosya, "10:48:12,BILGI,Kullanici sisteme giris yapti.\n");
        fclose(dosya);
    }
}

/* * 2. ADIM: LÝSTEYE ELEMAN EKLEME (YÜKSEK PERFORMANSLI)
 * Gelen verilerle yeni düðüm oluþturulur ve listenin en sonuna baðlanýr.
 */
void listeye_ekle(char* okunan_zaman, char* okunan_seviye, char* okunan_mesaj) {
    // Bellekte yeni vagon (düðüm) için yer ayýr
    struct SyslogDugumu* yeni_dugum = (struct SyslogDugumu*)malloc(sizeof(struct SyslogDugumu));
    if (yeni_dugum == NULL) {
        printf("Kritik Hata: Bellek ayrilamadi!\n");
        return;
    }

    // Verileri düðüme yerleþtir
    yeni_dugum->id = log_sayaci++;
    strcpy(yeni_dugum->zaman, okunan_zaman);
    strcpy(yeni_dugum->seviye, okunan_seviye);
    strcpy(yeni_dugum->mesaj, okunan_mesaj);
    yeni_dugum->sonraki = NULL;

    // Baðlama Ýþlemi (Tail iþaretçisi sayesinde anýnda sona eklenir)
    if (ilk_dugum == NULL) {
        ilk_dugum = yeni_dugum;
        son_dugum = yeni_dugum;
    } else {
        son_dugum->sonraki = yeni_dugum; // Eski son düðümü yeniye baðla
        son_dugum = yeni_dugum;          // Yeni düðümü "son düðüm" olarak güncelle
    }
}

/* * 3. ADIM: DOSYADAN OKUMA VE PARÇALAMA (PARSING)
 * Syslog verilerini anlamlý parçalara böler.
 */
void loglari_dosyadan_oku() {
    FILE* dosya = fopen("syslog_verileri.txt", "r");
    if (dosya == NULL) {
        printf("Hata: Dosya okunamadi!\n");
        return;
    }

    char satir[300];
    while (fgets(satir, sizeof(satir), dosya)) {
        satir[strcspn(satir, "\n")] = 0; // Satýr sonundaki enter karakterini sil

        // Virgüllere göre metni 3 parçaya böl (Zaman, Seviye, Mesaj)
        char* zaman_bilgisi = strtok(satir, ",");
        char* seviye_bilgisi = strtok(NULL, ",");
        char* mesaj_bilgisi = strtok(NULL, ",");

        // Baþarýyla bölündüyse baðlý listeye ekle
        if (zaman_bilgisi && seviye_bilgisi && mesaj_bilgisi) {
            listeye_ekle(zaman_bilgisi, seviye_bilgisi, mesaj_bilgisi);
        }
    }
    fclose(dosya);
}

/* * 4. ADIM: LÝSTEYÝ EKRANA YAZDIRMA
 */
void ekrana_yazdir() {
    struct SyslogDugumu* gecici = ilk_dugum;
    printf("\n--- LINUX SISTEM GUNLUKLERI (SYSLOG) ---\n");
    while (gecici != NULL) {
        printf("[%d] Saat: %s | Durum: %s | Olay: %s\n", 
               gecici->id, gecici->zaman, gecici->seviye, gecici->mesaj);
        gecici = gecici->sonraki;
    }
    printf("----------------------------------------\n");
}

/* * 5. ADIM: BELLEK TEMÝZLÝÐÝ (MEMORY LEAK ÖNLEME)
 * Program kapanýrken RAM'de ayrýlan tüm alanlarý iþletim sistemine iade eder.
 */
void bellegi_temizle() {
    struct SyslogDugumu* gecici;
    while (ilk_dugum != NULL) {
        gecici = ilk_dugum;
        ilk_dugum = ilk_dugum->sonraki;
        free(gecici);
    }
    son_dugum = NULL;
}

int main() {
    // Sistemin çalýþma akýþý
    test_dosyasi_olustur(); // Yoktan dosya var et
    loglari_dosyadan_oku(); // Dosyayý oku ve baðlý listeye aktar
    ekrana_yazdir();        // Listeyi ekranda göster
    bellegi_temizle();      // Çöpleri topla ve programý bitir

    return 0;
}
