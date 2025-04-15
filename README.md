# ✈️ Air Route Analyser

This project is an early-stage implementation of an airline route analysis tool written in **C**. The program reads flight route data from a text file, organizes the routes into periods (by year and month), and allows the user to perform several analyses such as:

📈 tracking route changes over time  
📊 displaying connectivity histograms for ports  
💰 comparing route costs

> ⚠️ **Note:** This code is a work in progress and contains many mistakes and omissions. It was developed as part of a learning exercise and still falls short of full functionality and coding standards.
> This repository reflects my learning process in working with data structures in C.
> I welcome any feedback or suggestions — improvements are ongoing!

---

## 🧩 Overview

The project makes use of **Abstract Data Types (ADTs)** to model:
- 🧭 **Routes** — individual flight data
- 📅 **Periods** — clusters of routes within a given month
- 📚 **Collection** — a series of periods sorted chronologically

### Key Features:
- 📆 **Route Changes:** Displays number of routes over a time range.
- 🏙️ **Most Connected Port:** Shows a histogram of connections for each port.
- 💸 **Route Costs:** Finds min and max current cost between two ports.

---

## 🛠️ Code Improvements and Style Considerations

During development, some improvements were made to follow better programming practices:

- 🐍 **Naming Conventions:** Unified all variable/function names using `snake_case`, in line with C standards.
- 📌 **Variable Declarations:** Declared all variables at the start of functions with explanatory comments.
- 🧾 **Commenting:** Added meaningful header comments and inline comments to improve code readability.

> ✍️ *Despite these changes, this remains an early-stage project with many areas for improvement.*
