
# Signature Scanner (Command line)

## Chức năng chính:

    - Quét tất cả các file có trong path thư mục mà người dùng nhập vào dựa trên signature bằng mã hex từ đó sẽ quét xem signature có tồn tại trong file hay không rồi in kết quả ra màn hình (Không quét file lớn hơn 50MB).


## Flow của chương trình:

    - Người dùng nhập vào đường dẫn đến folder cần quét.
    - Người dùng nhập signature cần tìm bằng mã hex (Không chứa dấu space).
    - Người dùng ấn Enter.
    - Chương trình bắt đầu quét và sau khi quét xong sẽ hiển thị thông báo và kết quả lên màn hình.

## Vai trò của đa luồng: 
    - Người dùng có thể thao tác với các API khác mà không cần đợi thread trước đó hoàn thành nhiệm vụ. 
    - Chia công việc quản lý thuận lợi hơn, giúp giải quyết vấn đề nhanh chóng vượt trội.

## Cấu trúc file:
    - Định nghĩa class thread pool trong "thread.h" và cài đặt trong "thread.cpp"
    - Các hàm lấy files và so sánh từng file với signature được nhập vào được cài đặt trong "main.cpp"
    - Project1: chứa source code của chương trình
    - Debug: chứa các file release đã build.

## Đa luồng:

### Ưu điểm:
    - Đa luồng giúp chia công việc chạy song song nhau, cùng nhau làm việc mà không cần phải đợi công việc trước đó thực hiện xong.
    - Mỗi luồng có thể sử dụng chung tài nguyên hoặc độc lập với nhau giúp tiết kiệm thời gian hoàn thành chương trình.

### Nhược điểm:
    - Khi xử lý nhiều luồng nếu lập trình viên mà không kiểm soát tốt các luồng của mình dẫn đến tranh chấp bộ nhớ, deadlock, thậm chí crash chương trình nếu tạo quá nhiều thread.

### Cơ chế chống xung đột trong project này:
    - Sử dụng mutex lock để đảm bảo không có tranh chấp trong việc sử dụng các thread.
