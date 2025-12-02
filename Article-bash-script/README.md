# Article Bash Script

A Bash script for scraping news articles from YNetNews and analyzing mentions of Israeli political figures.

## Overview

This project contains a web scraping script that:
1. Fetches article URLs from YNetNews politics category
2. Analyzes each article for mentions of specific political figures
3. Outputs results to a CSV file with occurrence counts

## Files

- **`scrape_news.sh`** - Main bash script for scraping and analyzing articles
- **`results.csv`** - Output file containing scraped results
- **`HW1`** - Assignment/homework reference file

## Features

The script analyzes articles for mentions of the following political figures:
- Netanyahu
- Gantz
- Bennett
- Perez

## How to Run

### Prerequisites
- `bash` shell
- `wget` - for fetching web content
- `grep` - for pattern matching (typically pre-installed)
- `sed` - for text processing (typically pre-installed)

### Execution

Make the script executable and run it:

```bash
chmod +x scrape_news.sh
./scrape_news.sh
```

The script will automatically:
1. Connect to YNetNews politics section
2. Extract all unique article URLs
3. Download and analyze each article
4. Write results to `results.csv`

## Output Format

The script generates `results.csv` with the following structure:

```
<number_of_articles>
<article_url>,Netanyahu,<count>,Gantz,<count>,Bennett,<count>,Perez,<count>
<article_url>,Netanyahu,<count>,Gantz,<count>,Bennett,<count>,Perez,<count>
...
```

If an article contains none of the tracked names, the output will be:
```
<article_url>,-
```

### Example Output

```
25
https://www.ynetnews.com/article/abc123,Netanyahu,3,Gantz,1,Bennett,0,Perez,2
https://www.ynetnews.com/article/def456,-
https://www.ynetnews.com/article/ghi789,Netanyahu,5,Gantz,2,Bennett,1,Perez,0
```

## How It Works

1. **Fetch Article List**: Uses `wget` to download the politics category page
2. **Extract URLs**: Parses HTML with `grep` regex to find article URLs
3. **Remove Duplicates**: Uses `sort | uniq` to get unique articles
4. **Count Names**: For each article:
   - Downloads article content
   - Extracts article body text
   - Counts occurrences of each political figure's name
5. **Write Results**: Appends to `results.csv` with counts or `-` if no names found

## Technical Details

### Script Logic
- Uses `--no-check-certificate` flag with `wget` for HTTPS connections
- Employs regex pattern matching with `grep -oP` for URL extraction
- Utilizes `sed` for HTML tag removal and text extraction
- Implements array iteration in Bash for processing multiple articles

### Performance Considerations
- Downloads each article twice (once for name counting)
- Network-intensive operation - runtime depends on number of articles and connection speed
- Results are appended incrementally to preserve data if script is interrupted

## Limitations

- Requires active internet connection
- Depends on YNetNews website structure remaining consistent
- Case-sensitive name matching
- Does not handle network errors gracefully

## Notes

- The script uses `2>/dev/null` to suppress `wget` error messages for cleaner output
- Empty articles or articles without the specified names will show a single `-` in the output
- The first line of the CSV contains the total count of articles processed
