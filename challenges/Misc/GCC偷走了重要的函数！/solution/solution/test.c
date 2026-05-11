
int main() {
  __asm__("xor rax, rax;"
          "inc rax;"
          "mov rcx, 0x0a646c72;"
          "push rcx;"
          "mov rcx, 0x6f57206f6c6c6548;"
          "push rcx;"

          "xor rdi, rdi;"
          "inc rdi;"
          "mov rsi, rsp;"

          "mov rdx, 12;"
          "syscall;"

          "mov rax, 60;"
          "xor rdi, rdi;"
          "syscall;");
}
