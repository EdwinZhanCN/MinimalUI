import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "MinimalUI",
  description: "A Cross-Platform UI Framework for SPI Displays - Supporting ESP32, STM32, and Jetson platforms",
  base: '/',
  cleanUrls: true,
  
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Getting Started', link: '/getting-started' },
      { text: 'Architecture', link: '/architecture' },
      { text: 'Platforms', link: '/platforms/' },
      { text: 'API Reference', link: '/api/' },
      { text: 'Examples', link: '/examples/' }
    ],

    sidebar: {
      '/': [
        {
          text: 'Introduction',
          items: [
            { text: 'Overview', link: '/' },
            { text: 'Getting Started', link: '/getting-started' },
            { text: 'Project Architecture', link: '/architecture' }
          ]
        },
        {
          text: 'Platform Support',
          items: [
            { text: 'Platform Overview', link: '/platforms/' },
            { text: 'ESP32 Platform', link: '/platforms/esp32' },
            { text: 'STM32 Platform', link: '/platforms/stm32' },
            { text: 'Jetson Platform', link: '/platforms/jetson' }
          ]
        },
        {
          text: 'Development Guide',
          items: [
            { text: 'ESP32 Setup', link: '/development/esp32-setup' },
            { text: 'Hardware Connections', link: '/development/hardware' },
            { text: 'Troubleshooting', link: '/development/troubleshooting' }
          ]
        },
        {
          text: 'API Reference',
          items: [
            { text: 'Graphics Driver', link: '/api/graphics-driver' },
            { text: 'Display Controllers', link: '/api/display-controllers' },
            { text: 'UI Components', link: '/api/ui-components' }
          ]
        },
        {
          text: 'Examples',
          items: [
            { text: 'Basic Examples', link: '/examples/' },
            { text: 'ESP32 OLED Demo', link: '/examples/esp32-oled' },
            { text: 'Custom Components', link: '/examples/custom-components' }
          ]
        }
      ]
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/MinimalUI/MinimalUI' }
    ],

    footer: {
      message: 'Released under the MIT License.',
      copyright: 'Copyright © 2024 MinimalUI Contributors'
    },

    search: {
      provider: 'local'
    }
  },

  // Add automatic translation support
  head: [
    ['script', {}, `
      if (typeof window !== 'undefined' && !window.location.search.includes('_immersive_translate_auto_translate=1')) {
        const url = new URL(window.location);
        url.searchParams.set('_immersive_translate_auto_translate', '1');
        window.history.replaceState({}, '', url.toString());
      }
    `]
  ],

  transformPageData(pageData) {
    return pageData
  }
})
