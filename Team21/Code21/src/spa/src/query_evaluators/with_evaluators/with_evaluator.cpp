
    // std::vector<AssignStatement*>* getAllAssignStatements();
    // std::vector<IfStatement*>* getAllIfStatements();
    // std::vector<WhileStatement*>* getAllWhileStatements();
    // std::vector<CallStatement*>* getAllCallStatements();
    // std::vector<ReadStatement*>* getAllReadStatements();
    // std::vector<PrintStatement*>* getAllPrintStatements();
    // std::vector<Procedure*>* getAllProcedures();
    // std::vector<Variable*>* getAllVariables();
    // std::vector<Constant*>* getAllConstants();
    // std::vector<Statement*>* getAllStatements();

    // std::vector<ParentRelationship*>* getParentRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    // std::vector<FollowsRelationship*>* getFollowsRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    // std::vector<ModifiesRelationship*>* getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    // std::vector<UsesRelationship*>* getUsesRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    // std::vector<ParentStarRelationship*>* getParentStarRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    // std::vector<FollowsStarRelationship*>* getFollowsStarRelationshipsByLeftAndRightEntityTypes(EntityType leftEntityType, EntityType rightEntityType);
    // std::vector<CallsRelationship*>* getAllCallsRelationships();
    // std::vector<CallsStarRelationship*>* getAllCallsStarRelationships();

    // std::unordered_set<WhileStatement*>* getWhileStatementsUsingVariableInCondition(std::string variableName);
    // std::unordered_set<IfStatement*>* getIfStatementsUsingVariableInCondition(std::string variableName);

    // ModifiesRelationship* getStatementModifiesVariableRelationship(int statementNumber, std::string variableName);
    // ModifiesRelationship* getProcedureModifiesVariableRelationship(std::string procedureName, std::string variableName);
    // UsesRelationship* getStatementUsesVariableRelationship(int statementNumber, std::string variableName);
    // UsesRelationship* getProcedureUsesVariableRelationship(std::string procedureName, std::string variableName);
    // ParentRelationship* getParentRelationship(int parentStatementNumber, int childStatementNumber);
    // FollowsRelationship* getFollowsRelationship(int firstStatementNumber, int secondStatementNumber);
    // ParentStarRelationship* getParentStarRelationship(int parentStatementNumber, int childStatementNumber);
    // FollowsStarRelationship* getFollowsStarRelationship(int firstStatementNumber, int secondStatementNumber);
    // CallsRelationship* getCallsRelationship(std::string callerName, std::string calleeName);
    // CallsStarRelationship* getCallsStarRelationship(std::string callerName, std::string calleeName);

    // Entity* getEntity(EntityType entityType, int entityValue);

    // Entity* getEntity(EntityType entityType, std::string entityValue);

    // std::vector<Entity*>* getEntitiesByType(EntityType entityType);

    // Relationship* getRelationship(RelationshipType relationshipType, EntityType leftEntityType, int leftEntityValue, EntityType rightEntityType, int rightEntityValue);

    // Relationship* getRelationship(RelationshipType relationshipType, EntityType leftEntityType, std::string leftEntityValue, EntityType rightEntityType, std::string rightEntityValue);

    // Relationship* getRelationship(RelationshipType relationshipType, EntityType leftEntityType, int leftEntityValue, EntityType rightEntityType, std::string rightEntityValue);

    // Relationship* getRelationship(RelationshipType relationshipType, EntityType leftEntityType, std::string leftEntityValue, EntityType rightEntityType, int rightEntityValue);

    // std::vector<Relationship*>* getRelationshipsByTypes(RelationshipType relationshipType, EntityType leftEntityType, EntityType rightEntityType);

    // std::vector<Entity*>* getRelationshipsByLeftEntityTypeAndRightEntityLiteral(RelationshipType relationshipType, EntityType leftEntityType, EntityType rightEntityType, int rightEntityValue);

    // std::vector<Entity*>* getRelationshipsByLeftEntityTypeAndRightEntityLiteral(RelationshipType relationshipType, EntityType leftEntityType, EntityType rightEntityType, std::string rightEntityValue);

    // std::vector<Entity*>* getRelationshipsByLeftEntityLiteralAndRightEntityType(RelationshipType relationshipType, EntityType leftEntityType, int leftEntityValue, EntityType rightEntityType);

    // std::vector<Entity*>* getRelationshipsByLeftEntityLiteralAndRightEntityType(RelationshipType relationshipType, EntityType leftEntityType, std::string leftEntityValue, EntityType rightEntityType);

    // cfg::CFG* getCFG();