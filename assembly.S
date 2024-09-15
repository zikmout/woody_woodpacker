.section .text
.global _start

# Define the decryption function
decrypt_section:
    push rbx                     # Save rbx register
    push rdi                     # Save rdi register
    push rsi                     # Save rsi register

    mov rdi, [rsp+32]             # Address of encrypted section (1st argument)
    mov rsi, [rsp+40]             # Size of the encrypted section (2nd argument)
    mov rbx, [rsp+48]             # Encryption key (3rd argument)

decrypt_loop:
    cmp rsi, 0                    # Check if the size is zero
    je decrypt_done               # If done, exit the loop

    mov al, [rdi]                 # Load a byte from the encrypted section
    xor al, bl                    # XOR the byte with the key
    mov [rdi], al                 # Write the decrypted byte back

    inc rdi                       # Move to the next byte
    dec rsi                       # Decrease the size
    jmp decrypt_loop              # Repeat the loop

decrypt_done:
    pop rsi                       # Restore rsi register
    pop rdi                       # Restore rdi register
    pop rbx                       # Restore rbx register
    ret                           # Return to caller

# Entry point of the new ELF binary
_start:
    mov rdi, <encrypted_section_addr>   # Encrypted section address
    mov rsi, <encrypted_section_size>   # Section size
    mov rbx, <encryption_key>           # Key used during encryption

    call decrypt_section                # Call the decryption function
    jmp <original_entry_point>          # Jump to the original entry point
