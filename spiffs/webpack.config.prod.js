const webpack = require('webpack');
const path = require('path');
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CompressionWebpackPlugin = require('compression-webpack-plugin');
const UglifyJsPlugin = require('uglifyjs-webpack-plugin');
const DelWebpackPlugin = require('del-webpack-plugin');
const { BundleAnalyzerPlugin } = require('webpack-bundle-analyzer');

const ROOT_DIR = path.resolve(__dirname, '../');
const DIST_DIR = path.resolve(ROOT_DIR, 'spiffs/dist');

const prodConfig = {
    mode: 'production',
    target: 'web',
    entry: './src/client/client.js',
    output: {
        path: DIST_DIR,
        filename: 'bundle.js'
    },
    module: {
        rules: [{
            test: /\.(sa|sc|c)ss$/,
            use: [
                MiniCssExtractPlugin.loader,
                'css-loader',
                'sass-loader',
            ],
        },{
            test: /\.(js|jsx)$/,
            loader: "babel-loader",
            exclude: /(node_modules)/,
            options: {
                presets: ['@babel/react', '@babel/env']
            }
        }]
    },
    optimization: {
        minimizer: [
          new UglifyJsPlugin({
            cache: true,
            uglifyOptions: {
                warnings: false,
                console: false,
                mangle: true,
                output: null,
                toplevel: false,
                nameCache: null,
                ie8: false,
                keep_fnames: false,
            }
          })
        ]
    },
    plugins: [
        new MiniCssExtractPlugin({
            filename: "[name].css",
            allChunks: false
        }),
        new CompressionWebpackPlugin({
            filename: '[path].gz[query]',
            algorithm: 'gzip',
            test: new RegExp('\\.(js|scss)$'),
            cache: true
        }),
        new DelWebpackPlugin({
          include: ['bundle.js'],
          keepGeneratedAssets: false,
          info: true,
        })
    ]
};

if (process.env.NODE_ANALYZE) {
  prodConfig.plugins.push(new BundleAnalyzerPlugin());
}

module.exports = prodConfig;