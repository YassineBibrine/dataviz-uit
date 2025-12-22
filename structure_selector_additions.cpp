// Add these two methods to src/ui/structure_selector.cpp after the onClearInteractiveClicked() method

void StructureSelector::setDataModelManager(DataModelManager* manager) {
    dataManager = manager;
  
    // Check if there are no structures - create samples automatically on first load
    if (dataManager && dataManager->getAllStructures().empty()) {
dataManager->createSampleStructures();
    }
  
    refreshStructureList();
}

void StructureSelector::onCreateSamplesClicked() {
    if (!dataManager) return;
    
    // Check if samples already exist
    auto structures = dataManager->getAllStructures();
    bool hasSamples = false;
    for (const auto& meta : structures) {
        if (meta.id.find("sample_") == 0) {
            hasSamples = true;
            break;
        }
    }
    
    if (hasSamples) {
        auto reply = QMessageBox::question(this, "Samples Exist",
            "Sample structures already exist. Create new ones anyway?\n"
            "(This will create additional sample structures)",
    QMessageBox::Yes | QMessageBox::No);
        
        if (reply != QMessageBox::Yes) {
 return;
        }
    }
    
    // Create samples
    auto createdIds = dataManager->createSampleStructures();
    
    // Refresh the list
    refreshStructureList();
    
    // Show success message
    QMessageBox::information(this, "Samples Created",
QString("Created %1 sample structures:\n"
     "• Sample Array\n"
          "• Sample Linked List\n"
       "• Sample Binary Tree\n"
      "• Sample Graph")
      .arg(createdIds.size()));
    
    // Emit signal
    emit samplesCreated();
}
