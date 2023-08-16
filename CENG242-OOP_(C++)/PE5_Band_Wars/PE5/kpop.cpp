#include "kpop.h"
#include "metal.h"
#include "rock.h"
#include "jazz.h"


int KPopBand::play(MusicBand *other)
{
    double C;
    if (dynamic_cast<KPopBand*>(other) != NULL)
    {
        C = 2.0; // C = 2.0 when versus KPopBand
    }
    else
    {
        C = 0.5; // C = 0.5 otherwise
    }
    int currentEnergy = get_energy();
    int talent = get_talent();
    int fans = get_fan_count();
    double score = (fans + 0.1 * talent * currentEnergy) * C;
    int remainingEnergy = currentEnergy - currentEnergy * 0.2; // K = 0.2 for KPopBand
    set_energy(remainingEnergy);
    return score;
}

void KPopBand::rehearse(void) 
{
    int currentEnergy = get_energy();
    int remainingEnergy = currentEnergy - currentEnergy * 0.5 * 0.2; // K = 0.2 for KPopBand
    set_energy(remainingEnergy);
    // No talent change for KPopBand (T = 0)
}