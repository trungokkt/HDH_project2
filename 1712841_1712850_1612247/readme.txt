-----------------Linux kernel module----------------
Mở terminal và chuyển thư mục hiện hành vào thư mục random. Sau đó, chạy lệnh sau để biên dịch module
$ make

Sau khi module được biên dịch xong, bạn có thể thêm module vào hệ thống
sudo insmod driver.ko

Sau khi module được thêm vào hệ thống, bạn có thể xem log kernel hệ thống bằng lệnh
$ dmesg

Để chạy thử tiến trình ở user space đọc character driver, code có đính kèm theo một chương trình kiểm thử phần này. Đầu tiên bạn cần biên dịch với một lệnh đơn giản như sau
$ gcc test.c -o test

Sau đó, bạn nên chạy tệp thực thi dưới quyền root để có thể đọc được character driver một cách ổn định.
Sau khi chạy được, bạn có thể nhấn phím Enter (hay Return) để đọc những số ngẫu được sinh ra trong module, để thoát nhấn Ctrl-C
$ sudo ./test

Để gỡ cài đặt module, bạn chạy lệnh
$ sudo rmmod driver.ko

Nếu bạn muốn xem lại quá trình chạy của module, bạn có thể chạy dmesg để xem lại
$ dmesg

------------------------Hook system call------------------

Mở terminal và chuyển thư mục hiện hành vào thư mục hook/. Sau đó, chạy lệnh sau để biên dịch hook
$ make

Sau khi hook được biên dịch xong, bạn có thể thêm hook vào hệ thống
sudo insmod hook.ko

Lúc này, bạn có thể xem log kernel để bắt toàn bộ system call open và write được gọi bằng dmesg. Với những bản dmesg mới hơn, bạn có thể xem dmesg liên tục bằng cách gõ lệnh sau vào terminal
$ dmesg -w

Để xem dmesg liên tục trên những phiên bản cũ, bạn có thể chạy lệnh sau
$ watch "dmesg | tail -20"

Sau khi theo dõi hoàn tất và muốn gỡ cài đặt hook, bạn chạy lệnh sau
$ sudo rmmod hook.ko
