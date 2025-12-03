#include "execution_state.h"
#include <algorithm> // std::max

// ===============================
// Comparaisons
// ===============================
void ExecutionState::recordComparison(const std::string& a, const std::string& b)
{
    (void)a;
    (void)b;
    comparisonCount++;
    metricsMap["comparisons"] = comparisonCount;
}

// ===============================
// Échanges (swap)
// ===============================
void ExecutionState::recordSwap(const std::string& a, const std::string& b)
{
    (void)a;
    (void)b;
    swapCount++;
    metricsMap["swaps"] = swapCount;
}

// ===============================
// Accès simple (lecture/écriture)
// ===============================
void ExecutionState::recordAccess(const std::string& a)
{
    (void)a;
    metricsMap["access"]++;
}

// ===============================
// Enregistrement générique d'une opération
// ===============================
void ExecutionState::recordOperation(const std::string& operation, const std::map<std::string,std::string>& details)
{
    // compteur dynamique pour l'opération
    metricsMap[operation]++;

    // mettre à jour les compteurs dédiés si présents
    if (operation == "comparisons") {
        comparisonCount = metricsMap[operation];
    } else if (operation == "swaps") {
        swapCount = metricsMap[operation];
    }

    // sauvegarder directement un snapshot avec les détails (optionnel)
    history.push_back({ currentStep, metricsMap, details });
    totalSteps = std::max(totalSteps, currentStep);
}

// ===============================
// Sauvegarde d’un snapshot complet
// ===============================
void ExecutionState::saveState()
{
    history.push_back({ currentStep, metricsMap, {} });
    totalSteps = std::max(totalSteps, currentStep);
}

// ===============================
// Restauration d’un état antérieur
// ===============================
void ExecutionState::restoreState(int step)
{
    if (step < 0 || step >= static_cast<int>(history.size()))
        return;

    currentStep = step;

    // restaurer les métriques depuis l'historique
    metricsMap = history[step].metrics;

    // remettre à jour champs rapides
    auto itComp = metricsMap.find("comparisons");
    comparisonCount = (itComp != metricsMap.end()) ? itComp->second : 0;

    auto itSwap = metricsMap.find("swaps");
    swapCount = (itSwap != metricsMap.end()) ? itSwap->second : 0;
}

// ===============================
// Retourne toutes les métriques importantes
// ===============================
std::map<std::string,int> ExecutionState::getMetrics() const
{
    auto copy = metricsMap;
    copy["step"] = currentStep;
    copy["totalSteps"] = totalSteps;
    return copy;
}

int ExecutionState::getCurrentStep() const
{
    return currentStep;
}

int ExecutionState::getTotalSteps() const
{
    return totalSteps;
}

// ===============================
// Avancer d'une étape
// ===============================
void ExecutionState::advanceStep()
{
    ++currentStep;
    totalSteps = std::max(totalSteps, currentStep);
}

// ===============================
// Réinitialisation totale
// ===============================
void ExecutionState::reset()
{
    currentStep = 0;
    totalSteps = 0;
    comparisonCount = 0;
    swapCount = 0;

    metricsMap.clear();
    metricsMap["comparisons"] = 0;
    metricsMap["swaps"] = 0;
    metricsMap["access"] = 0;

    history.clear();
}
