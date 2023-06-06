library(utils)
library(dygraphs)
library(htmlwidgets)

plotHist <- function(...){
  dataList <- list(...)
  driverDf <- dataList[[1]]

  values <- list()
  for(df in dataList){
    valName <- colnames(df)[2]
    values <- cbind(values, df[[valName]])
    colnames(values)[ncol(values)] <- valName
  }

  timeInc <- seq(from=1, to=nrow(driverDf))
  resultPlot <- dygraph(data.frame(time=timeInc, value=values))
  resultPlot <- dyAxis(resultPlot, "y", drawGrid=FALSE)
  resultPlot <- dyAxis(resultPlot, "x", drawGrid=FALSE)
  resultPlot <- dyLegend(resultPlot, width=400)

  metaData = as.vector(driverDf$meta)
  dateData = as.vector(driverDf$date)

  for(i in seq(from=1, to=length(metaData))) {
    dateStr <- strsplit(dateData[i], ' ')[[1]][1]
    if(metaData[i]=='c') {
      resultPlot <- dyEvent(resultPlot, i, label=NULL, labelLoc=c("top", "bottom"),
			    color="lightslategray", strokePatter="dotted")
    } else if (metaData[i]=='w') {
      resultPlot <- dyEvent(resultPlot, i, label=dateStr, labelLoc="bottom",
                            color="blackslategray", strokePatter="dashed")
    } else if (metaData[i]=='a') {
      lbl = paste("announcement", dateStr)
      resultPlot <- dyEvent(resultPlot, i, label=lbl, labelLoc="bottom",
                            color="black", strokePatter="solid")
    } else if (metaData[i]=='e') {
      lbl = paste("effective", dateStr)
      resultPlot <- dyEvent(resultPlot, i, label=lbl, labelLoc="bottom",
                            color="black", strokePatter="solid")
    }
  }

  for(df in dataList) {
    valName <- colnames(df)[2]
    metaData = as.vector(df$meta)

    seriesName <- valName
    if(length(dataList) > 1) {
      seriesName <- paste("value", valName, sep='.')
    }

    for(i in seq(from=1,to=length(metaData))) {
      if(metaData[i] %in% c('s', 'u', 'f', 'm', 'q', 'A', 'p')) {
        resultPlot <- dyAnnotation(resultPlot, i+1, toupper(metaData[i]), width=14, height=14,
				   tickHeight=14, tooltip='strike', series=seriesName)
      }
    }
  }

  resultPlot <- dyOptions(resultPlot, colors=list("red", "black", "blue", "gray", "silver"))
  return(resultPlot)
}

SPX_HEDGED_2023_Q1 <- read.csv("./SPX500_HEDGED_2023_Q1.csv")
SPX_HEDGED_2022_Q4 <- read.csv("./SPX500_HEDGED_2022_Q4.csv")
SPX_HEDGED_2022_Q3 <- read.csv("./SPX500_HEDGED_2022_Q3.csv")
SPX_HEDGED_2022_Q2 <- read.csv("./SPX500_HEDGED_2022_Q2.csv")
SPX_HEDGED_2022_Q1 <- read.csv("./SPX500_HEDGED_2022_Q1.csv")

g <- plotHist(SPX_HEDGED_2023_Q1, SPX_HEDGED_2022_Q4, SPX_HEDGED_2022_Q3, SPX_HEDGED_2022_Q2, SPX_HEDGED_2022_Q1)

saveWidget(g, "./SPX.html", title="SPX")
