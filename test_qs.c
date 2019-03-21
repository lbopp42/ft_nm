#include <stdio.h>
#include <unistd.h>

void	print_tab(int tab[])
{
    int i;

    for(i = 0; i < 12; i++)
    {
		printf("%d ", tab[i]);
    }
    putchar('\n');
}

void echanger(int tableau[], int a, int b)
{
    int temp = tableau[a];
    tableau[a] = tableau[b];
    tableau[b] = temp;
}

void quickSort(int tableau[], int debut, int fin)
{
    int gauche = debut-1;
    int droite = fin+1;
    const int pivot = tableau[debut];

    /* Si le tableau est de longueur nulle, il n'y a rien à faire. */
    if(debut >= fin)
        return;

    /* Sinon, on parcourt le tableau, une fois de droite à gauche, et une
       autre de gauche à droite, à la recherche d'éléments mal placés,
       que l'on permute. Si les deux parcours se croisent, on arrête. */
    while(1)
    {
		printf("==============================\n");
		printf("pivot = %d\n", pivot);
		printf("tableau[droite] = %d\n", tableau[droite - 1]);
		printf("tableau[gauche] = %d\n", tableau[gauche + 1]);
        do droite--; while(tableau[droite] > pivot);
        do gauche++; while(tableau[gauche] < pivot);
		printf("On compare [%d] > [%d]\n", tableau[droite], tableau[gauche]);

        if(gauche < droite)
		{
			printf("On change\n");
            echanger(tableau, gauche, droite);
		}
        else break;
		print_tab(tableau);
		printf("==============================\n");
    }

	print_tab(tableau);
	printf("----------------------------------------------\n");
    /* Maintenant, tous les éléments inférieurs au pivot sont avant ceux
       supérieurs au pivot. On a donc deux groupes de cases à trier. On utilise
       pour cela... la méthode quickSort elle-même ! */
    quickSort(tableau, debut, droite);
    quickSort(tableau, droite+1, fin);
}

int main(void)
{
    int tab[12] = {5, 3, 4, 1, 2, 50, 10, 12, 43, 32, 27, 1};

    quickSort(tab, 0, 11);
	print_tab(tab);
    
    return 0;
}
