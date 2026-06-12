#include <zephyr/kernel.h>

// Contador para simular diferentes tipos de pão
static volatile int saldo_vitrine = 0;

int padeiro_thread(void)
{
    while (1) {        
        // Simula tempo para fazer o pão
        k_msleep(1000);  // 1 segundo para fazer pão
        
        // Faz o pão e coloca na vitrine
        saldo_vitrine += 1;
        
        printk("\nPADEIRO: Pao pronto\nVitrine com %d pao(es)\n", saldo_vitrine);
    }
}

void cliente_thread(void)
{
    while (1) {
         // Simula tempo para retirar o pão
        k_msleep(1500);  // 1.5 segundos

        // Pega o pão do buffer
        saldo_vitrine -= 1;
        
        printk("\nCLIENTE: Peguei pao\nVitrine com %d pao(es)\n", saldo_vitrine);
    }
}

// Definir as threads
K_THREAD_DEFINE(padeiro_tid, 512, padeiro_thread, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(cliente_tid, 512, cliente_thread, NULL, NULL, NULL, 7, 0, 0);

int main(void)
{
    printk("=== Produtor-Consumidor ===\n");
    printk("Padeiro produz, Cliente compra\n");
    
    while (1) {
        k_sleep(K_SECONDS(10));
        printk("--- Sistema funcionando ---\n");
    }

    return 0;
}