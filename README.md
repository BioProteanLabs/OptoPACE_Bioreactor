# OptoPACE_Bioreactor

The OptoPACE Bioreactor: An Accessible Turbidostat System with Phage-Assisted Continuous Evolution Capability and Optogenetic Controls
Authors
1. Crane, Ryan D.* 
2. Isaacs, William T.*
3. Ijaz, Manal*
4. Michelle Kim
5. Nicole Salazar
6. Brianna Botello
7. Flores-Mora, Fernando E.
8. Bartelle, Benjamin B.

*co-first

Abstract:
Continuous culture systems are essential for synthetic biology techniques like Directed Evolution (DE) and Phage-Assisted Continuous Evolution (PACE), which require metabolically active bacterial cultures for extended periods. Turbidostats, which maintain cell density using real-time optical density (OD) feedback, offer ideal conditions for inducing gene expression during mid-log phase, which is the optimal growth stage for synthetic circuit activity. However, current PACE-compatible bioreactors are either cost-prohibitive or require significant technical expertise. Here, we present the OptoPACE Bioreactor (OPB), a low-cost, user-friendly turbidostat system optimized for PACE and optogenetic control. The OPB uses a 650 nm laser and photoresistor to measure turbidity, an Arduino microcontroller for feedback control, and NeoPixel LEDs to enable multispectral optogenetic stimulation. Designed with 3D-printed components and off-the-shelf electronics, the system operates with a single peristaltic pump and uses the siphon effect to simplify fluidics. A custom motor control shield and an LCD-rotary encoder interface enable code-free operation. The OPB supports dual bioreactors connected unidirectionally, ensuring continuous culture while maintaining precise control over optical inputs and nutrient flow. The entire system can be built for under $200, significantly lowering the barrier to entry for labs interested in continuous evolution. By combining affordability, modularity, and ease of use, the OptoPACE Bioreactor provides a robust platform for protein engineering and synthetic biology, enabling broader adoption of advanced continuous evolution methods like PACE.

<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/OpenSystem.png" alt="Assembly Step 1" width="500">


Metadata Overview:
Main design files: https://github.com/BioProteanLabs/OptoPACE_Bioreactor/tree/main
Target group: undergraduate researchers, under-resourced laboratories in synthetic biology, students interested in biological hardware
Skills required: 3d printing - easy; software implementation - easy; physical assembly - intermediate; electrical assembly - advanced


Introduction:
Continuous bacterial culture bioreactors are widely employed in biological research, with recent applications in the DE of biomolecules [1, 3]. In a typical DE experiment, bioreactors  maintain a population of microorganisms in a metabolically active state to support growth and expression of synthetic biological programs. For methods like PACE, two tandem bioreactors are needed: one for growing a substrate culture that feeds into a second “lagoon” to support bacteriophages actively evolving. 
For PACE, maintaining the substrate culture at a steady state gives consistent conditions for DE, requiring either a chemostat or turbidostat control system. Chemostat bioreactors are simpler in that they supply a steady state of nutrients, while a turbidostat makes continual adjustments based on feedback—namely, culture density or turbidity [8, 9, 17]. Turbidostats allow for more optimal and customizable culture conditions but require continual monitoring and control hardware for adjustments based on feedback.
Turbidity is typically measured by optical density (OD), which reflects both light absorption by molecules in the culture, such as pigments, and light scattering from materials with different refractive indexes, typically cell membrane lipids. OD serves as a real-time quantitative assay of cell concentration, allowing researchers to infer the growth stage of the bacterial population. In a bacterial culture, OD measurements typically begin with exponential growth, reach mid-log phase, and eventually flatten out into “death phase” as resources become limited. For the production of recombinant proteins at experimental and industrial scales, mid-log phase is considered the optimal state for inducing protein expression or a synthetic biological program. In a turbidostat bioreactor, continuous monitoring of bacterial growth allows users to identify the mid-log phase, input nutrients to maintain this optimal point, and induce expression of biological circuits.
Induction of biological circuits presents another layer of control required in a bioreactor. Historically, this has meant adding specific sugars or their chemical analog to regulate bacterial operons to initiate expression [2, 16]. This requires an additional chemostat input for the induction agent. More recently, optogenetic circuits—based on biology from plants and cyanobacteria—allow for the induction of gene expression using light. Current developments in synthetic biology have even enabled wavelength-specific optogenetic circuits. With stable culture conditions and multiple biological control inputs, bioreactors could control several genetic circuits simultaneously [4, 10, 11, 14].
Despite their potential, experimental-scale turbidostat bioreactors are not commonly available. The literature indicates that establishing a functional PACE workflow often requires years of effort due to the dual challenges of designing effective synthetic biological circuits and building reliable hardware to support culture [12, 13]. Additional obstacles—such as extensive setup time and the need for frequent maintenance due to microbial biofilm accumulation on culture surfaces—make developing an automated PACE workflow even more difficult to implement.

The OptoPACE Bioreactor is designed for optogenetic control of synthetic biological circuits.


The OptoPACE Bioreactor uses the siphon effect to minimize the number of pumps needed.


The OptoPACE Bioreactor uses a 650nm laser and a photoresistor to measure turbidity, and NeoPixel LEDs for multispectral control.

<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/SiphonSystem.png" alt="Assembly Step 1" width="500">


The OptoPACE Bioreactor is controlled by a single Arduino microcontroller and a custom designed motor control shield.

The OptoPACE Bioreactor uses an LCD screen and rotary encoder as an interface.

<img src="https://raw.githubusercontent.com/BioProteanLabs/OptoPACE_Bioreactor/main/Images/StressTest.jpeg" alt="Assembly Step 1" width="500">


