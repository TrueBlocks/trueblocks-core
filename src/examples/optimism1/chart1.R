library(ggplot2)
library(readr)
library(scales)

data <- read_csv("/Users/jrush/Development/trueblocks-core/build/data1.csv")

plot <- ggplot(data, aes(x = bn, y = secs)) + 
  geom_line(aes(color = secs), size = .8) +
  scale_color_gradient(low = "blue", high = "orange") +
  labs(
    title = "Seconds Per Block",
    subtitle = "Prior to and Immediately Following Bedrock",
    x = "block num",
    y = "seconds / block",
    caption = "created using TrueBlocks and R."
  ) +
  theme_minimal() +
  theme(
    plot.title = element_text(face = "bold", size = 20, hjust = 0.5),
    plot.subtitle = element_text(size = 16, hjust = 0.5),
    axis.title = element_text(size = 12),
    axis.text = element_text(size = 10),
    legend.position = "right",
    legend.title = element_text(size = 14),
    legend.text = element_text(size = 10),
    plot.margin = margin(t = 30, r = 30, b = 30, l = 30),
    plot.background = element_rect(color = "black", size = 2, fill = NA),
    panel.border = element_rect(color = "black", linewidth = .5, fill = NA)
  ) +
  scale_x_continuous(labels = scales::label_number())

# Print the plot
plot
