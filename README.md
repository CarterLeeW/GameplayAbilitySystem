<h1>Unreal Engine 5 Gameplay Ability System Demo</h1>

<p>
  This repository is for a top-down rpg game concept that I have created using Unreal Engine 5 showcasing the Gameplay Ability System (GAS), which is a powerful plugin that allows rapid and robust development for building abilities and actions that actors can own and trigger; see the <a href="https://dev.epicgames.com/documentation/en-us/unreal-engine/understanding-the-unreal-engine-gameplay-ability-system">documentation</a> for more information. 
</p>
<p>
  I began this project in early 2023 as a learning tool for Unreal Engine 5 and the Gameplay Ability System, and it has since also become a platform for me to explore other aspects of game development in a dynamic setting, such as UI, AI scripting, and UE Materials.
</p>
<p>
  I encourage the reader to view my other repositories on GitHub, as well as my <a href="https://carterleew.itch.io/">Itch IO webpage</a>, which contains downloadable and playable versions and demo videos of this and other game projects.
</p>

<h2>Project Features</h2>

<h4>Gameplay Ability System</h4>
<p>
  The Gameplay Ability System is the heart of most of the game's interaction and functionality. Each Pawn gets a Player State, an Ability System Component, and an Attribute Set. Gameplay Abilities are activated by a pawn and cause Gameplay Effects that act on another pawn's attributes. Damage can be caused using a damage Gameplay Effect, which can use attributes such as resistances to modify the final damage and trigger other effects. For multiplayer games, Gameplay Cues are used to handle cosmetic effects such as sounds and particles.
</p>

<h4>User Interface</h4>
<p>
  The UI in this project features both the Model-View-Controller (MVC) and Model-View-ViewModel (MVVM) design patters to create responsive, event driven user interfaces that can communicate with the player's Ability System Component and Attribute Set.
</p>

<h4>AI</h4>
<p>
  Enemy AI, while not the focus of this project, is functional and allows Enemies to choose which gameplay abilities to activate based on environment factors, and can be easily scaled to use attributes such as health and mana to influence the decision process.
</p>

