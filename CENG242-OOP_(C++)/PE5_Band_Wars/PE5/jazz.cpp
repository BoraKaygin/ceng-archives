#include "kpop.h"
#include "metal.h"
#include "rock.h"
#include "jazz.h"


int JazzBand::play(MusicBand *other)
{
    double C;
    if (dynamic_cast<KPopBand*>(other) != NULL)
    {
        C = 0.5; // C = 0.5 when versus KPopBand
    }
    else if (dynamic_cast<MetalBand*>(other) != NULL)
    {
        C = 1.3; // C = 1.3 when versus MetalBand
    }
    else if (dynamic_cast<RockBand*>(other) != NULL)
    {
        C = 0.7; // C = 0.7 when versus RockBand
    }
    else if (dynamic_cast<JazzBand*>(other) != NULL)
    {
        C = 0.7; // C = 0.7 when versus JazzBand
    }
    int currentEnergy = get_energy();
    int talent = get_talent();
    int fans = get_fan_count();
    double score = (fans + 0.1 * talent * currentEnergy) * C;
    int remainingEnergy = currentEnergy - currentEnergy * 0.06; // K = 0.06 for JazzBand
    set_energy(remainingEnergy);
    return score;
}

void JazzBand::rehearse(void) 
{
    int currentEnergy = get_energy();
    int remainingEnergy = currentEnergy - currentEnergy * 0.5 * 0.06; // K = 0.06 for JazzBand
    set_energy(remainingEnergy);
    int currentTalent = get_talent();
    int newTalent = currentTalent + 5; // T = 5 for JazzBand
    set_talent(newTalent);
}