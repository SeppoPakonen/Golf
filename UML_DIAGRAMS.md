# UML Class Diagrams for Golf Project

This project includes several UML class diagrams created with PlantUML to visualize the architecture:

## Diagrams

1. **Rude Engine Core Classes** (`rude_engine_classes.puml`)
   - Core framework classes including RudeControl, RudeGame, RudeGL, etc.
   - UI control hierarchy and rendering system

2. **Rude Engine Physics Classes** (`rude_physics_classes.puml`)
   - Physics simulation classes (RudePhysics, RudePhysicsObject, etc.)
   - Collision detection system

3. **Golf Example Application Classes** (`golf_example_classes.puml`)
   - Game-specific classes (RBGame, RBGameBase, RBUITitle, etc.)
   - Game UI and logic components

4. **Rude Engine Utility Classes** (`rude_utilities_classes.puml`)
   - Utility classes for debugging, performance, file handling, etc.
   - Supporting systems like registry, tweaker, etc.

## View Diagrams

To view these diagrams, you can:
- Use a PlantUML viewer/editor online (like plantuml.com)
- Install PlantUML plugin in your IDE (VSCode, IntelliJ, etc.)
- Use the PlantUML jar file with Graphviz to generate images

## Architecture Overview

The project follows a game engine architecture with:
- A core engine layer (Rude Engine) providing basic functionality
- Physics and rendering systems
- Platform abstraction
- Game-specific implementation layer (Golf example)