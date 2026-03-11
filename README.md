# Linux Syslog Yöneticisi - Bağlı Liste Uygulaması

Bu proje, Linux sistemlerindeki günlüklerin (Syslog) hafızada nasıl tutulduğunu göstermek amacıyla C dili ile hazırlanmıştır. Sabit diziler yerine dinamik hafıza kullanılarak, veriler **Tek Yönlü Bağlı Liste** üzerinde tutulmaktadır.

## 🚀 Projenin Amacı ve Özellikleri
Sistem günlüklerinin boyutu önceden bilinemeyeceği için, hafızayı verimli kullanmak adına her kayıt sadece ihtiyaç anında oluşturulan "düğümler" içinde saklanır.

**Temel Özellikler:**
- **Tek Yönlü Bağlı Liste:** Kayıtlar zaman sırasına göre sürekli ileriye doğru aktığı için, sadece "sonraki" veriyi gösteren yapı kullanılmıştır.
- **Kuyruk İşaretçisi (Tail Pointer):** Yeni gelen kayıtların listenin en sonuna eklenmesi sırasında baştan sona tarama yapıp bilgisayarı yavaşlatmamak için özel bir işaretçi kullanılmıştır. Kayıtlar beklemeden anında sona eklenir.
- **Veri Parçalama:** Okunan metinler doğrudan ekrana basılmak yerine, saat, uyarı seviyesi ve hata mesajı olarak üç ayrı parçaya ayrılır.
- **Hafıza Temizliği:** Program kapanmadan önce açılan tüm bellek alanları sisteme iade edilerek hafıza tıkanıklığı (memory leak) engellenir.

## 🛠️ Kurulum ve Çalıştırma
Uygulama, çalıştırıldığı klasörde otomatik olarak `syslog_verileri.txt` adında örnek bir test dosyası oluşturur ve bu dosyayı okur. Dışarıdan dosya eklemenize gerek yoktur.

**Çalıştırmak İçin:**
`bagli_liste_syslog.c` dosyasını kullandığınız C derleyicisinde (örneğin Dev-C++) açıp "Derle ve Çalıştır" (F11) yapmanız yeterlidir.

## 📋 Çıktı Örneği
Program çalıştığında ekranda şu şekilde yapısal hale getirilmiş logları göreceksiniz:
```text
--- LINUX SISTEM GUNLUKLERI (SYSLOG) ---
[1] Saat: 10:45:01 | Durum: BILGI | Olay: Sistem basariyla onyuklendi.
[2] Saat: 10:45:05 | Durum: UYARI | Olay: USB cihazi algilandi ancak taninmadi.
