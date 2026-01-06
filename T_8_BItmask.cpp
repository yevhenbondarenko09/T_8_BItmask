#include <iostream>
#include <bitset> // Для зручного виводу бітів на екран
#include <thread>
#include <chrono>
#include <windows.h>

using namespace std;

// Макроси для роботи з бітами
#define BIT(n)          (1U << (n))
#define SET_BIT(reg, n) ((reg) |= BIT(n))
#define CLR_BIT(reg, n) ((reg) &= ~BIT(n))
#define TGL_BIT(reg, n) ((reg) ^= BIT(n))

/**
 * Симуляція 8-бітного регістру GPIO
 * В реальному мікроконтролері це була б конкретна адреса в пам'яті.
 */
unsigned char GPIO_REG = 0b00000000;

/**
 * Функція для красивого виводу стану регістру
 */
void print_gpio_state(const char* label) {
    cout << label << ": \t"
        << bitset<8>(GPIO_REG)
        << " (Dec: " << (int)GPIO_REG << ")" << endl;
}

bool CHECK_BIT(int pin, unsigned char REG) {
    if (REG & BIT(pin)) {
        return true;
    }
   return false;
   }

void set_bus_value(int value) {
    GPIO_REG &= 0b11110000;
    unsigned char safe_value = value & 0b00001111;
    GPIO_REG |= safe_value;
}

int main() {
    SetConsoleOutputCP(1251);
    print_gpio_state("Початковий стан   ");

    // 1. Встановлюємо 0-й та 7-й піни (SET)
    // Уявімо, що на 0-му піні підключено світлодіод
    cout << "\n--- Встановлюємо піни 0 та 7 ---" << endl;
    SET_BIT(GPIO_REG, 0);
    SET_BIT(GPIO_REG, 7);
    print_gpio_state("Після SET_BIT     ");

    // 2. Інвертуємо 0-й пін (TOGGLE)
    // Якщо світлодіод горів — він згасне
    cout << "\n--- Інвертуємо пін 0 ---" << endl;
    TGL_BIT(GPIO_REG, 0);
    print_gpio_state("Після TGL_BIT     ");

    // 3. Скидаємо 7-й пін (CLEAR)
    cout << "\n--- Скидаємо пін 7 ---" << endl;
    CLR_BIT(GPIO_REG, 7);
    print_gpio_state("Після CLR_BIT     ");

    // 4. Складна маска (встановлення кількох бітів одразу)
    // Встановимо піни 2, 3, 4 через АБО з маскою
    cout << "\n--- Встановлюємо групу пінів (2,3,4) ---" << endl;
    unsigned char mask = 0b00011100;
    GPIO_REG |= mask;
    print_gpio_state("Після групової операції");

    // 5. Перевірка стану (CHECK)
    cout << "\n--- Перевірка стану піна 2 ---" << endl;
    if (GPIO_REG & BIT(2)) {
        cout << "Пін 2 знаходиться у стані HIGH (1)" << endl;
    }
    else {
        cout << "Пін 2 знаходиться у стані LOW (0)" << endl;
    }

    set_bus_value(2);
    print_gpio_state("Після установки маски 0010 на перші 4 біти    ");
    GPIO_REG = 16;
    print_gpio_state("Передав десяткове 16     ");
    GPIO_REG = 0;
    int n = 0;
       while (n < 5) {
        TGL_BIT(GPIO_REG, 5);
        print_gpio_state("Світлодіод горить     ");
        this_thread::sleep_for(chrono::seconds(1));
        TGL_BIT(GPIO_REG, 5);
        print_gpio_state("Світлодіод згас       ");
        this_thread::sleep_for(chrono::seconds(1));
        n++;
       };
    return 0;
}

/**
 * ПОЯСНЕННЯ ЛОГІКИ:
 * * 1. SET: reg |= (1 << n)
 * Використовуємо |, бо 1 | будь-що = 1. Інші біти не змінюються (0 | x = x).
 * * 2. CLR: reg &= ~(1 << n)
 * Спочатку створюємо маску, де всі біти 1, крім n-го (наприклад, 11110111).
 * Потім робимо &, тому n-й біт гарантовано стане 0, а інші залишаться як були.
 * * 3. TGL: reg ^= (1 << n)
 * XOR з одиницею завжди перевертає біт (0^1=1, 1^1=0).
 */