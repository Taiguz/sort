const fs = require('fs')
const Chart = require('chart.js')

class Pie {

    constructor(options) {
        this.name = options ? options.name : 'name'
        this.names = options ? options.names : []
        this.trocas = options ? options.trocas : null
        this.comparacoes = options ? options.comparacoes : null
        this.pizzas = document.getElementsByTagName('pizzas')[0]
        this.width = options ? options.width : '300px'
        this.height = options ? options.height : '300px'
        this.datasets = this.trocas ? this.trocas : this.comparacoes
        this.colors = options ? options.colors : []
        this.fatorx = 3
        this.fatory = 3
        this.create(options)
    }

    updateSize() {
        this.canvas.style.width = `${window.innerWidth / this.fatorx}px`
        this.canvas.style.height = `${window.innerHeight / this.fatory}px`

    }

    create(options) {
        this.canvas = document.createElement('canvas')
        this.canvas.setAttribute('id', this.name)
        this.pizzas.appendChild(this.canvas)

        this.chart = new Chart(this.canvas.getContext('2d'), {
            type: 'doughnut',
            data: {
                datasets: [{
                    data: this.datasets,
                    backgroundColor: this.colors
                }],

                labels: this.names
            },
            options: {

                title: {
                    display: true,
                    text: this.name,
                    fontSize: 26
                },
                legend: {
                    labels: {
                        fontColor: 'black',
                        fontSize: 24
                    }

                },
                responsive: false,
                padding: 5

            }

        });

    }
}
class Graph {

    constructor(options) {
        this.barras = document.getElementById('barras')
        this.estados = options ? options.estados : []
        this.totalComparacoes = options ? options.totalComparacoes : 0
        this.totalTrocas = options ? options.totalTrocas : 0
        this.animationDuration = options ? options.animationDuration : 50
        this.name = options ? options.name : ''
        this.titleSize = options ? options.titleSize : 26
        this.axisFontSize = options ? options.axisFontSize : 24
        this.defaultColor = options ? options.defaultColor : 'green'
        this.changeColor = options ? options.changeColor : 'red'
        this.compareColor = options ? options.compareColor : 'orange'
        this.endColor = options ? options.endColor : 'green'
        this.controller = options ? options.controller : null
        this.delay = options ? options.delay : 200
        this.fatorx = 3
        this.fatory = 3
        this.create(options)

    }
    updateSize() {
        this.canvas.style.width = `${window.innerWidth / this.fatorx}px`
        this.canvas.style.height = `${window.innerHeight / this.fatory}px`

    }
    create() {
        this.canvas = document.createElement('canvas')
        this.canvas.setAttribute('id', this.name)
        this.barras.appendChild(this.canvas)
        this.chart = new Chart(this.canvas.getContext('2d'), {
            type: 'bar',
            data: {
                labels: this.estados[0].estado,
                datasets: [{
                    data: this.estados[0].estado,
                    backgroundColor: [],
                    borderColor: 'green',
                    borderWidth: 0
                }]
            },
            options: {
                animation: {
                    duration: this.animationDuration
                },
                title: {
                    display: true,
                    text: this.name,
                    fontSize: this.titleSize
                },
                scales: {
                    xAxes: [{

                        barThickness: 4,
                        categoryPercentage: 1.0,
                        barPercentage: 0.8,
                        gridLines: {
                            offsetGridLines: false
                        },
                        ticks: {
                            fontSize: this.axisFontSize,

                        }
                    }],
                    yAxes: [{

                        ticks: {
                            display: false,
                            beginAtZero: true,
                            fontSize: 36
                        },
                        fontSize: 36
                    }]
                },
                tooltips: {

                    enabled: false,
                    bodyFontSize: 36,
                },
                padding: 5,
                responsive: false,
                legend: {
                    display: false,
                }
            }
        });

    }
    play() {

        return new Promise(() => {

            this.estados.forEach((estado, ind) => {

                if (ind == (this.estados.length - 1)) estado.last = true

                this.update(estado, this.delay * (ind + 1))

            })

        })

    }
    update(estado, tempo) {

        return new Promise(resolve => {

            let backgroundColor = []
            let [comp1, comp2] = estado.comparacao ? estado.comparacao : estado.troca

            if (estado.comparacao) {
                let data = this.chart.data.datasets[0].data
                data.forEach(est => {
                    if (est == data[comp1] || est == data[comp2]) {

                        estado.comparacao ? backgroundColor.push(this.compareColor) : backgroundColor.push(this.changeColor)

                    } else {
                        backgroundColor.push(this.defaultColor)
                    }

                })

            } else {

                estado.estado.forEach(est => {
                    if (est == estado.estado[comp1] || est == estado.estado[comp2]) {

                        estado.comparacao ? backgroundColor.push(this.compareColor) : backgroundColor.push(this.changeColor)

                    } else {
                        backgroundColor.push(this.defaultColor)
                    }

                })
            }
            setTimeout(() => {
                if (!estado.comparacao) {
                    this.chart.data.datasets[0].data = estado.estado
                    this.chart.data.labels = estado.estado
                }
                this.chart.data.datasets[0].backgroundColor = backgroundColor
                this.chart.update();
                if (estado.last) {
                    this.controller.addItem({
                        name: this.name,
                        totalTrocas: this.totalTrocas,
                        totalComparacoes: this.totalComparacoes

                    })
                    setTimeout(() => {
                        this.chart.data.datasets[0].backgroundColor = estado.estado.map(est => this.endColor)
                        this.chart.update();


                    }, tempo)
                }
            }, tempo)
        })
    }

}
class Controller {

    constructor(options) {
        this.body = document.getElementsByTagName('body')[0]
        this.ranktrocas = document.getElementsByTagName('ranktrocas')[0]
        this.rankcomparacoes = document.getElementsByTagName('rankcomparacoes')[0]
        this.files = []
        this.directory = options ? options.directory : 'resultados'
        this.animationDuration = options ? options.animationDuration : 50
        this.titleSize = options ? options.titleSize : 36
        this.axisFontSize = options ? options.axisFontSize : 24
        this.defaultColor = options ? options.defaultColor : 'green'
        this.changeColor = options ? options.changeColor : 'red'
        this.compareColor = options ? options.compareColor : 'orange'
        this.endColor = options ? options.endColor : 'green'
        this.delay = options ? options.delay : 100
        this.getFiles()
        this.graphs = []
        this.pies = []
        this.createGraphs()
        this.body.onresize = () => {
            console.log('resize')
            this.graphs.forEach(graph => {

                graph.updateSize()

            })
            this.pies.forEach(pie => {

                pie.updateSize()

            })
        }
        this.body.onresize()
        this.getRankBts()
        this.getGraphsBts()

    }
    getRankBts() {
        this.ranknav = document.getElementById('navrank')
        this.telasranking = Array.from(document.getElementsByTagName('ranking')[0].children)
            .filter(rank => rank.tagName != "NAV" && rank.tagName != "H2")
        this.navbts = this.ranknav.children
        Array.from(this.telasranking).forEach((tela, index) => {

            tela.style.display = 'none'
        })
        this.telasranking[0].style.display = 'flex'

        Array.from(this.navbts).forEach(btItem => {
            btItem.classList.remove('active')
        })
        this.navbts[0].classList.add('active')

        Array.from(this.navbts).forEach((bt, ind) => {

            bt.onclick = () => {

                Array.from(this.navbts).forEach(btItem => {
                    btItem.classList.remove('active')
                })

                bt.classList.add('active')

                Array.from(this.telasranking).forEach((tela, index) => {

                    if (index != ind) tela.style.display = 'none'
                })

                this.telasranking[ind].style.display = 'flex'

            }

        })


    }
    getGraphsBts() {
        this.graphnav = document.getElementById('navgraphs')
        this.telasgraph = Array.from(document.getElementsByTagName('graficos')[0].children)
            .filter(rank => rank.tagName != "NAV" && rank.tagName != "H2")
        this.graphbts = this.graphnav.children
        Array.from(this.telasgraph).forEach((tela, index) => {

            tela.style.display = 'none'
        })
        this.telasgraph[0].style.display = 'flex'

        Array.from(this.graphbts).forEach(btItem => {
            btItem.classList.remove('active')
        })
        this.graphbts[0].classList.add('active')

        Array.from(this.graphbts).forEach((bt, ind) => {

            bt.onclick = () => {

                Array.from(this.graphbts).forEach(btItem => {
                    btItem.classList.remove('active')
                })

                bt.classList.add('active')

                Array.from(this.telasgraph).forEach((tela, index) => {

                    if (index != ind) tela.style.display = 'none'
                })

                this.telasgraph[ind].style.display = 'flex'

            }

        })
    }
    getFiles() {

        this.files = fs.readdirSync(`./${this.directory}`)
        this.files = this.files.map(file => {
            let name = file.split('.')[0]
            let jsonContent = require(`./${this.directory}/${file}`)
            return { name, jsonContent }
        })
    }
    getRandomColor() {
        var letters = '0123456789ABCDEF';
        var color = '#';
        for (var i = 0; i < 6; i++) {
            color += letters[Math.floor(Math.random() * 16)];
        }
        return color;
    }
    fillColors(length) {
        let colors = []
        let color = this.getRandomColor()
        let find = false
        while (colors.length != length) {
            find = colors.find(c => color == c)
            if (!find) colors.push(color)
            color = this.getRandomColor()
        }
        return colors

    }
    createGraphs() {

        let names = []
        let trocas = []
        let comparacoes = []

        this.files.forEach(file => {
            const { estados, totalTrocas, totalComparacoes } = file.jsonContent
            trocas.push(totalTrocas)
            comparacoes.push(totalComparacoes)
            names.push(file.name)
            this.graphs.push(new Graph({
                estados,
                totalComparacoes,
                totalTrocas,
                animationDuration: this.animationDuration,
                name: file.name,
                titleSize: this.titleSize,
                axisFontSize: this.axisFontSize,
                defaultColor: this.defaultColor,
                compareColor: this.compareColor,
                changeColor: this.changeColor,
                delay: this.delay,
                endColor: this.endColor,
                controller: this

            }))

        })

        let colors = this.fillColors(this.files.length)

        this.pies.push(new Pie({

            name: 'Comparações',
            comparacoes,
            names,
            colors

        }))
        this.pies.push(new Pie({

            name: 'Trocas',
            trocas,
            names,
            colors

        }))
    }
    addItem(ob) {



        let rankitem = document.createElement('rankitem')
        rankitem.innerHTML = `<item>${this.ranktrocas.children.length + 1}</item><item>${ob.name}</item><item>${ob.totalTrocas}</item>`
        this.ranktrocas.appendChild(rankitem)



        let rankitem2 = document.createElement('rankitem')
        rankitem2.innerHTML = `<item>${this.rankcomparacoes.children.length + 1}</item><item>${ob.name}</item><item>${ob.totalComparacoes}</item>`
        this.rankcomparacoes.appendChild(rankitem2)



    }
    resetRanking() {

        this.ranktrocas.innerHTML = ''
        this.rankcomparacoes.innerHTML = ''

    }
    playAll() {
        this.resetRanking()
        this.graphs.forEach(graph => {

            graph.play()
        })
    }
}
class Vetor {

    constructor(options) {

        this.vetor = document.getElementsByTagName('vetor')[0]
        this.drawVetor()
    }
    drawVetor() {
        this.vetor.textContent = `[${this.constructReverseVetor(50).join(',')}]`
        debugger
    }
    constructReverseVetor(length) {

        let vetor = []
        for (let i = (length - 1); i > 0; i--) {
            vetor.push(i)
        }
        return vetor

    }

}

let controller = new Controller({
    directory: 'resultados',
    animationDuration: 60,
    titleSize: 26,
    axisFontSize: 24,
    compareColor: 'orange',
    changeColor: 'red',
    defaultColor: 'green',
    endColor: '#006600',
    delay: 5
})

let vetor = new Vetor()

controller.playAll()



