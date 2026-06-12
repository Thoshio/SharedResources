#include <zephyr/kernel.h>

// Semáforo para sincronização
K_SEM_DEFINE(pode_comprar, 0, 1);  //inicia sem pão na vitrine (pode comprar? nao)
K_SEM_DEFINE(pode_colocar, 1, 1);  //inicia sem pão na vitrine (pode colocar? sim)

// Contador para simular diferentes tipos de pão
static volatile int saldo_vitrine = 0; //não há pão na vitrine

int padeiro_thread(void)
{
    while (1) {
        // Permissão para colocar
        k_sem_take(&pode_colocar, K_FOREVER);

        // Simula tempo para coloocar o pão na vitrine
        k_msleep(1000);  // 1 segundo
        
        // Coloca pão na vitrine
        saldo_vitrine += 1;
        printk("\nPADEIRO: Pao pronto\nVitrine com %d pao(es)\n", saldo_vitrine);

        // Depois de colocado, tem pelo menos 1 pao disponivel (já podem comprar)
        k_sem_give(&pode_comprar);

        if (saldo_vitrine < 10){
            // há espaço para pão
            k_sem_give(&pode_colocar);
        }
    }
}

void cliente_thread(void)
{
    while (1) {
        // Espera permissão para comprar
        k_sem_take(&pode_comprar, K_FOREVER);

        // Simula tempo para retirar o pão
        k_msleep(100);  // 1.5 segundos

        // Pega o pão
        saldo_vitrine -= 1;
        printk("\nCLIENTE: Peguei pao\nVitrine com %d pao(es)\n", saldo_vitrine);

        // Depois de comprado, tem pelo menos 1 vaga disponivel na vritrine (já podem colocar)
        k_sem_give(&pode_colocar);
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