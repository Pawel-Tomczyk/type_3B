#include "headers.h"

struct Prices {
    int tabaco;
    int paper;
    int matches;
};

struct Product {
    long mtype;
    int price;
};

int main(int argc, char *argv[]) {
    // Initializing the shared memory
    srand(time(NULL));
    int walet = rand() % 50 + 1;

    key_t key_prices = ftok("prices", 65);
    int shmid_prices = shmget(key_prices, sizeof(Prices), 0777);
    Prices *shm_prices = (Prices *)shmat(shmid_prices, NULL, 0);

    key_t key_tabaco = ftok("tabaco", 65);
    int msgid_tabaco = msgget(key_tabaco, 0777);
    key_t key_paper = ftok("paper", 65);
    int msgid_paper = msgget(key_paper, 0777);
    key_t key_matches = ftok("matches", 65);
    int msgid_matches = msgget(key_matches, 0777);
    if (msgid_tabaco == -1 || msgid_paper == -1 || msgid_matches == -1) {
        printf("msgget\n");
        exit(1);
    }

    Product product_m, product_p, product_t;
    struct msqid_ds info;

    while (true) {
        msgctl(msgid_paper, IPC_STAT, &info);
        while (info.msg_qnum > 0) {
            msgrcv(msgid_paper, &product_p, sizeof(product_p), 1, 0);
            walet += product_p.price;
            msgctl(msgid_matches, IPC_STAT, &info);

            printf("Walet: %d\n", walet);
        }

        if (shm_prices->matches + shm_prices->tabaco <= walet) {
            product_m.mtype = 1;
            product_m.price = shm_prices->matches;
            product_t.mtype = 1;
            product_t.price = shm_prices->tabaco;
            walet -= product_m.price;
            walet -= product_t.price;
            msgsnd(msgid_matches, &product_m, sizeof(product_m), 0);
            msgsnd(msgid_tabaco, &product_t, sizeof(product_t), 0);
            printf("\tPaid:\n");
            printf("\tMatches: %d\n\tTabaco: %d\n", product_m.price, product_t.price);
            printf("\tWalet: %d\n", walet);
            printf("\tSmoking...\n");
            sleep(5);
        }
    }

    return 0;
}